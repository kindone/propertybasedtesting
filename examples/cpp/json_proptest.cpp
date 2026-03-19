#include "proptest/proptest.hpp"
#include "proptest/stateful/stateful_function.hpp"
#include "proptest/test/gtest.hpp"

#include <nlohmann/json.hpp>
#include <sstream>

using json = nlohmann::json;
using namespace proptest;
using namespace proptest::stateful;

// ============================================================================
// Helper: generators for JSON-friendly values
// ============================================================================

static auto smallIntGen()  { return gen::interval<int>(-1000, 1000); }
static auto smallUintGen() { return gen::interval<int>(0, 100); }
static auto keyGen()       { return gen::string(1, 8); }

// ============================================================================
// 1. Roundtrip: parse(dump(j)) == j
//    The fundamental property of any serializer.
// ============================================================================

TEST(JsonProperty, IntRoundtrip)
{
    EXPECT_FOR_ALL([](int value) {
        json j = value;
        auto text = j.dump();
        auto parsed = json::parse(text);
        PROP_ASSERT(parsed == j);
        PROP_ASSERT(parsed.is_number_integer());
    });
}

TEST(JsonProperty, DoubleRoundtripExact)
{
    auto finiteDouble = gen::interval<int>(-100000, 100000).map<double>(
        [](int n) { return n * 0.01; });

    EXPECT_FOR_ALL([](double value) {
        json j = value;
        auto text = j.dump(-1, ' ', false, json::error_handler_t::strict);
        auto parsed = json::parse(text);
        PROP_ASSERT(parsed == j);
    }, finiteDouble);
}

TEST(JsonProperty, StringRoundtrip)
{
    EXPECT_FOR_ALL([](string value) {
        json j = value;
        auto text = j.dump();
        auto parsed = json::parse(text);
        PROP_ASSERT(parsed == j);
        PROP_ASSERT_EQ(parsed.get<string>(), value);
    });
}

TEST(JsonProperty, BoolRoundtrip)
{
    EXPECT_FOR_ALL([](bool value) {
        json j = value;
        auto text = j.dump();
        auto parsed = json::parse(text);
        PROP_ASSERT(parsed == j);
        PROP_ASSERT(parsed.is_boolean());
    });
}

TEST(JsonProperty, NullRoundtrip)
{
    json j = nullptr;
    auto text = j.dump();
    auto parsed = json::parse(text);
    EXPECT_TRUE(parsed.is_null());
    EXPECT_EQ(parsed, j);
}

// ============================================================================
// 2. Array properties
// ============================================================================

TEST(JsonProperty, ArrayPushBackIncreasesSize)
{
    auto sizeGen = smallUintGen();

    EXPECT_FOR_ALL([](int count) {
        json arr = json::array();
        for (int i = 0; i < count; i++)
            arr.push_back(i);
        PROP_ASSERT(arr.is_array());
        PROP_ASSERT_EQ(static_cast<int>(arr.size()), count);
    }, sizeGen);
}

TEST(JsonProperty, ArrayRoundtrip)
{
    EXPECT_FOR_ALL([](vector<int> values) {
        json arr = values;
        auto text = arr.dump();
        auto parsed = json::parse(text);
        PROP_ASSERT(parsed == arr);
        auto back = parsed.get<vector<int>>();
        PROP_ASSERT(back == values);
    });
}

TEST(JsonProperty, ArrayConcatSize)
{
    EXPECT_FOR_ALL([](vector<int> a, vector<int> b) {
        json ja = a;
        json jb = b;
        json merged = json::array();
        for (auto& elem : ja) merged.push_back(elem);
        for (auto& elem : jb) merged.push_back(elem);
        PROP_ASSERT(merged.size() == ja.size() + jb.size());
    });
}

// ============================================================================
// 3. Object properties
// ============================================================================

TEST(JsonProperty, ObjectInsertAndRetrieve)
{
    EXPECT_FOR_ALL([](string key, int value) {
        json obj = json::object();
        obj[key] = value;
        PROP_ASSERT(obj.contains(key));
        PROP_ASSERT_EQ(obj[key].get<int>(), value);
    });
}

TEST(JsonProperty, ObjectEraseRemovesKey)
{
    EXPECT_FOR_ALL([](string key, int value) {
        json obj = json::object();
        obj[key] = value;
        PROP_ASSERT(obj.contains(key));
        obj.erase(key);
        PROP_ASSERT(!obj.contains(key));
        PROP_ASSERT(obj.empty());
    });
}

TEST(JsonProperty, ObjectMergeContainsAllKeys)
{
    EXPECT_FOR_ALL([](string k1, string k2, int v1, int v2) {
        json a = json::object();
        json b = json::object();
        a[k1] = v1;
        b[k2] = v2;

        json merged = a;
        merged.update(b);

        PROP_ASSERT(merged.contains(k2));
        PROP_ASSERT_EQ(merged[k2].get<int>(), v2);
        if (k1 != k2) {
            PROP_ASSERT(merged.contains(k1));
        }
    });
}

// ============================================================================
// 4. Type preservation properties
// ============================================================================

TEST(JsonProperty, TypePreservationAfterCopy)
{
    EXPECT_FOR_ALL([](int intVal, string strVal, bool boolVal) {
        json original = json::object();
        original["int"] = intVal;
        original["str"] = strVal;
        original["bool"] = boolVal;
        original["null"] = nullptr;
        original["arr"] = json::array({1, 2, 3});

        json copy = original;

        PROP_ASSERT(copy["int"].is_number_integer());
        PROP_ASSERT(copy["str"].is_string());
        PROP_ASSERT(copy["bool"].is_boolean());
        PROP_ASSERT(copy["null"].is_null());
        PROP_ASSERT(copy["arr"].is_array());
        PROP_ASSERT(copy == original);
    });
}

// ============================================================================
// 5. Comparison / ordering properties
// ============================================================================

TEST(JsonProperty, EqualityReflexive)
{
    EXPECT_FOR_ALL([](int value) {
        json j = value;
        PROP_ASSERT(j == j);
    });
}

TEST(JsonProperty, EqualitySymmetric)
{
    EXPECT_FOR_ALL([](int value) {
        json a = value;
        json b = value;
        PROP_ASSERT(a == b);
        PROP_ASSERT(b == a);
    });
}

// ============================================================================
// 6. Serialisation format properties
// ============================================================================

TEST(JsonProperty, DumpIndentProducesNewlines)
{
    EXPECT_FOR_ALL([](vector<int> values) {
        if (values.size() < 2)
            return;
        json j = values;
        auto compact = j.dump();
        auto pretty  = j.dump(2);
        PROP_ASSERT(pretty.size() > compact.size());
        PROP_ASSERT(pretty.find('\n') != string::npos);
    });
}

// ============================================================================
// 7. Stateful test: JSON object as a key-value store
//    Tests insert / overwrite / erase / contains / size against a
//    reference std::map model.
// ============================================================================

struct JsonObjModel {
    std::map<string, int> expected;
};

using JsonObjAction = Action<json, JsonObjModel>;

TEST(JsonStateful, ObjectAsKeyValueStore)
{
    auto insertGen = gen::pair(keyGen(), smallIntGen())
        .map([](const pair<string, int>& kv) -> JsonObjAction {
            auto [k, v] = kv;
            return JsonObjAction(
                PROP_ACTION_NAME("Insert", k, v),
                [k, v](json& obj, JsonObjModel& model) {
                    obj[k] = v;
                    model.expected[k] = v;
                    PROP_ASSERT(obj.contains(k));
                    PROP_ASSERT_EQ(obj[k].get<int>(), v);
                });
        });

    auto eraseGen = keyGen().map([](const string& k) -> JsonObjAction {
        return JsonObjAction(
            PROP_ACTION_NAME("Erase", k),
            [k](json& obj, JsonObjModel& model) {
                obj.erase(k);
                model.expected.erase(k);
                PROP_ASSERT(!obj.contains(k));
            });
    });

    auto containsGen = keyGen().map([](const string& k) -> JsonObjAction {
        return JsonObjAction(
            PROP_ACTION_NAME("Contains", k),
            [k](json& obj, JsonObjModel& model) {
                bool inJson  = obj.contains(k);
                bool inModel = model.expected.count(k) > 0;
                PROP_ASSERT(inJson == inModel);
            });
    });

    auto roundtripAction = JsonObjAction(
        "Roundtrip",
        [](json& obj, JsonObjModel&) {
            auto text = obj.dump();
            auto parsed = json::parse(text);
            PROP_ASSERT(parsed == obj);
        });

    auto actionGen = gen::oneOf<JsonObjAction>(
        insertGen, eraseGen, containsGen, roundtripAction);

    auto prop = statefulProperty<json, JsonObjModel>(
        gen::just(json::object()),
        [](json&) -> JsonObjModel { return JsonObjModel{}; },
        actionGen);

    prop.setPostCheck([](json& obj, JsonObjModel& model) {
            PROP_ASSERT(obj.size() == model.expected.size());
            for (auto& [k, v] : model.expected) {
                PROP_ASSERT(obj.contains(k));
                PROP_ASSERT_EQ(obj[k].get<int>(), v);
            }
            auto text = obj.dump();
            auto parsed = json::parse(text);
            PROP_ASSERT(parsed == obj);
        })
        .setSeed(0)
        .setNumRuns(500)
        .setActionListMinSize(5)
        .setActionListMaxSize(30)
        .go();
}

// ============================================================================
// 8. Stateful test: JSON array with push/pop/access against a
//    reference vector model.
// ============================================================================

struct JsonArrModel {
    vector<int> expected;
};

using JsonArrAction = Action<json, JsonArrModel>;

TEST(JsonStateful, ArrayPushPopAccess)
{
    auto pushGen = smallIntGen().map([](int value) -> JsonArrAction {
        return JsonArrAction(
            PROP_ACTION_NAME("PushBack", value),
            [value](json& arr, JsonArrModel& model) {
                arr.push_back(value);
                model.expected.push_back(value);
            });
    });

    auto popAction = JsonArrAction("PopBack", [](json& arr, JsonArrModel& model) {
        if (arr.empty()) {
            PROP_ASSERT(model.expected.empty());
            return;
        }
        arr.erase(arr.end() - 1);
        model.expected.pop_back();
    });

    auto accessGen = smallUintGen().map([](int idx) -> JsonArrAction {
        return JsonArrAction(
            PROP_ACTION_NAME("Access", idx),
            [idx](json& arr, JsonArrModel& model) {
                if (model.expected.empty()) return;
                size_t i = static_cast<size_t>(idx) % model.expected.size();
                PROP_ASSERT_EQ(arr[i].get<int>(), model.expected[i]);
            });
    });

    auto sizeAction = JsonArrAction("Size", [](json& arr, JsonArrModel& model) {
        PROP_ASSERT(arr.size() == model.expected.size());
    });

    auto roundtripAction = JsonArrAction(
        "Roundtrip",
        [](json& arr, JsonArrModel&) {
            auto parsed = json::parse(arr.dump());
            PROP_ASSERT(parsed == arr);
        });

    auto actionGen = gen::oneOf<JsonArrAction>(
        pushGen, popAction, accessGen, sizeAction, roundtripAction);

    auto prop = statefulProperty<json, JsonArrModel>(
        gen::just(json::array()),
        [](json&) -> JsonArrModel { return JsonArrModel{}; },
        actionGen);

    prop.setPostCheck([](json& arr, JsonArrModel& model) {
            PROP_ASSERT(arr.size() == model.expected.size());
            for (size_t i = 0; i < model.expected.size(); i++)
                PROP_ASSERT_EQ(arr[i].get<int>(), model.expected[i]);
            auto parsed = json::parse(arr.dump());
            PROP_ASSERT(parsed == arr);
        })
        .setSeed(0)
        .setNumRuns(500)
        .setActionListMinSize(5)
        .setActionListMaxSize(30)
        .go();
}
