#include "proptest/proptest.hpp"
#include "proptest/stateful/stateful_function.hpp"
#include "proptest/test/gtest.hpp"

#include <stack>

using namespace proptest;
using namespace proptest::stateful;

// ============================================================================
// Example 1: Stack — simple stateful test (no model)
//
// Generates random push/pop/top sequences on std::stack<int> and asserts
// that basic invariants hold after every action.
// ============================================================================

TEST(StatefulStack, SimpleNoModel)
{
    using Stack = std::stack<int>;

    auto pushGen = gen::interval<int>(-1000, 1000).map<SimpleAction<Stack>>([](int value) {
        return SimpleAction<Stack>(
            PROP_ACTION_NAME("Push", value),
            [value](Stack& s) {
                auto prevSize = s.size();
                s.push(value);
                PROP_ASSERT(s.size() == prevSize + 1);
                PROP_ASSERT(s.top() == value);
            });
    });

    auto popAction = SimpleAction<Stack>("Pop", [](Stack& s) {
        if (s.empty())
            return;
        auto prevSize = s.size();
        s.pop();
        PROP_ASSERT(s.size() == prevSize - 1);
    });

    auto topAction = SimpleAction<Stack>("Top", [](Stack& s) {
        if (s.empty())
            return;
        [[maybe_unused]] auto val = s.top();
    });

    auto actionGen = gen::oneOf<SimpleAction<Stack>>(pushGen, popAction, topAction);
    auto prop = statefulProperty<Stack>(gen::just(Stack{}), actionGen);
    prop.setSeed(0).setNumRuns(200).go();
}

// ============================================================================
// Example 2: Stack with model — verifies the real stack against a
// reference model (std::vector used as the "expected" stack).
//
// After every action sequence the post-check asserts that the real stack
// and the model agree on size and top element.
// ============================================================================

struct StackModel {
    vector<int> expected;
};

TEST(StatefulStack, WithReferenceModel)
{
    using Stack = std::stack<int>;

    auto pushGen = gen::interval<int>(-1000, 1000).map<Action<Stack, StackModel>>([](int value) {
        return Action<Stack, StackModel>(
            PROP_ACTION_NAME("Push", value),
            [value](Stack& s, StackModel& model) {
                s.push(value);
                model.expected.push_back(value);
            });
    });

    auto popAction = Action<Stack, StackModel>("Pop", [](Stack& s, StackModel& model) {
        if (s.empty()) {
            PROP_ASSERT(model.expected.empty());
            return;
        }
        s.pop();
        model.expected.pop_back();
    });

    auto topAction = Action<Stack, StackModel>("Top", [](Stack& s, StackModel& model) {
        PROP_ASSERT(s.empty() == model.expected.empty());
        if (!s.empty()) {
            PROP_ASSERT_EQ(s.top(), model.expected.back());
        }
    });

    auto sizeAction = Action<Stack, StackModel>("Size", [](Stack& s, StackModel& model) {
        PROP_ASSERT(s.size() == model.expected.size());
    });

    auto actionGen = gen::oneOf<Action<Stack, StackModel>>(pushGen, popAction, topAction, sizeAction);

    auto prop = statefulProperty<Stack, StackModel>(
        gen::just(Stack{}),
        [](Stack&) -> StackModel { return StackModel{}; },
        actionGen);

    prop.setPostCheck([](Stack& s, StackModel& model) {
            PROP_ASSERT(s.size() == model.expected.size());
            if (!s.empty())
                PROP_ASSERT_EQ(s.top(), model.expected.back());
        })
        .setSeed(0)
        .setNumRuns(200)
        .go();
}

// ============================================================================
// Example 3: Bank account — model-based stateful test for a toy
// BankAccount class with deposit, withdraw, and balance checking.
// ============================================================================

class BankAccount {
public:
    explicit BankAccount(int initial) : balance_(initial) {}
    void deposit(int amount)  { balance_ += amount; }
    bool withdraw(int amount) {
        if (amount > balance_) return false;
        balance_ -= amount;
        return true;
    }
    int balance() const { return balance_; }
private:
    int balance_;
};

struct AccountModel {
    int expectedBalance;
};

TEST(StatefulBankAccount, DepositWithdrawBalance)
{
    auto depositGen = gen::interval<int>(1, 500).map<Action<BankAccount, AccountModel>>([](int amount) {
        return Action<BankAccount, AccountModel>(
            PROP_ACTION_NAME("Deposit", amount),
            [amount](BankAccount& acct, AccountModel& model) {
                acct.deposit(amount);
                model.expectedBalance += amount;
                PROP_ASSERT_EQ(acct.balance(), model.expectedBalance);
            });
    });

    auto withdrawGen = gen::interval<int>(1, 500).map<Action<BankAccount, AccountModel>>([](int amount) {
        return Action<BankAccount, AccountModel>(
            PROP_ACTION_NAME("Withdraw", amount),
            [amount](BankAccount& acct, AccountModel& model) {
                bool ok = acct.withdraw(amount);
                if (amount <= model.expectedBalance) {
                    PROP_ASSERT(ok);
                    model.expectedBalance -= amount;
                } else {
                    PROP_ASSERT(!ok);
                }
                PROP_ASSERT_EQ(acct.balance(), model.expectedBalance);
            });
    });

    auto balanceAction = Action<BankAccount, AccountModel>(
        "CheckBalance",
        [](BankAccount& acct, AccountModel& model) {
            PROP_ASSERT_EQ(acct.balance(), model.expectedBalance);
            PROP_ASSERT_GE(acct.balance(), 0);
        });

    auto actionGen = gen::oneOf<Action<BankAccount, AccountModel>>(
        depositGen, withdrawGen, balanceAction);

    auto initialBalanceGen = gen::interval<int>(0, 1000).map<BankAccount>([](int bal) {
        return BankAccount(bal);
    });

    auto prop = statefulProperty<BankAccount, AccountModel>(
        initialBalanceGen,
        [](BankAccount& acct) -> AccountModel { return AccountModel{acct.balance()}; },
        actionGen);

    prop.setPostCheck([](BankAccount& acct, AccountModel& model) {
            PROP_ASSERT_EQ(acct.balance(), model.expectedBalance);
            PROP_ASSERT_GE(acct.balance(), 0);
        })
        .setSeed(0)
        .setNumRuns(500)
        .setActionListMinSize(5)
        .setActionListMaxSize(30)
        .go();
}

// ============================================================================
// Example 4: std::set — model-based test checking insert/erase/contains
// against a reference vector.
// ============================================================================

struct SetModel {
    vector<int> elements;

    bool contains(int v) const {
        return std::find(elements.begin(), elements.end(), v) != elements.end();
    }
    void insert(int v) {
        if (!contains(v)) elements.push_back(v);
    }
    void erase(int v) {
        elements.erase(std::remove(elements.begin(), elements.end(), v), elements.end());
    }
};

TEST(StatefulSet, InsertEraseContains)
{
    using Set = std::set<int>;

    auto insertGen = gen::interval<int>(0, 50).map<Action<Set, SetModel>>([](int value) {
        return Action<Set, SetModel>(
            PROP_ACTION_NAME("Insert", value),
            [value](Set& s, SetModel& model) {
                s.insert(value);
                model.insert(value);
                PROP_ASSERT(s.count(value) == 1);
            });
    });

    auto eraseGen = gen::interval<int>(0, 50).map<Action<Set, SetModel>>([](int value) {
        return Action<Set, SetModel>(
            PROP_ACTION_NAME("Erase", value),
            [value](Set& s, SetModel& model) {
                s.erase(value);
                model.erase(value);
                PROP_ASSERT(s.count(value) == 0);
            });
    });

    auto containsGen = gen::interval<int>(0, 50).map<Action<Set, SetModel>>([](int value) {
        return Action<Set, SetModel>(
            PROP_ACTION_NAME("Contains", value),
            [value](Set& s, SetModel& model) {
                bool inSet   = s.count(value) > 0;
                bool inModel = model.contains(value);
                PROP_ASSERT(inSet == inModel);
            });
    });

    auto actionGen = gen::oneOf<Action<Set, SetModel>>(insertGen, eraseGen, containsGen);

    auto prop = statefulProperty<Set, SetModel>(
        gen::just(Set{}),
        [](Set&) -> SetModel { return SetModel{}; },
        actionGen);

    prop.setPostCheck([](Set& s, SetModel& model) {
            PROP_ASSERT(s.size() == model.elements.size());
            for (auto& elem : model.elements)
                PROP_ASSERT(s.count(elem) == 1);
        })
        .setSeed(0)
        .setNumRuns(300)
        .go();
}
