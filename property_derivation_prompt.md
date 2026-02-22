# Property Test Derivation: Reasoning Prompt

Use this prompt to guide an LLM through deriving property-based test candidates. The LLM should **show its reasoning process** step by step, not just produce answers.

---

## Prompt

You are helping derive property-based test candidates for a software component. **Think through this step by step, showing your reasoning at each stage.**

Here is the component to analyze:

```
[Paste code, API docs, design notes, or specification here]
```

---

### Your Process

**Step 1: Understand the component**

First, analyze what this component does:
- Is it stateless (pure function) or stateful (object with mutable state)?
- What are the inputs and outputs (or state + operations if stateful)?
- What can fail (allocator, I/O, invalid inputs)?

**Show your reasoning:** Write 2-3 sentences summarizing the component and its key characteristics.

---

**Step 2: Find property sources**

Look for where properties can come from. **Think through each source:**

- **Design/code**: What invariants, preconditions, or postconditions are stated or implied?
- **Documentation**: What guarantees are documented (e.g., "returns sorted", "O(n log n)", "thread-safe")?
- **Domain knowledge**: What mathematical or domain properties apply (commutativity, sorting semantics, container invariants)?
- **Reference**: Is there existing code that can serve as a test oracle (e.g., for refactoring)?

**Show your reasoning:** For each source that applies, briefly state what you found and how it could become a property. If something is unclear, note what assumption you're making.

---

**Step 3: Identify important subdomains**

Think about what inputs or action sequences matter most for finding bugs:

- **Edge cases**: Empty, zero, one element, min/max values, nullptr, full capacity
- **Error paths**: Invalid arguments, out-of-range, failing dependencies
- **Stateful sequences**: If stateful, what action sequences are interesting (empty→push, many ops, interleaved, stress tests)?

**Show your reasoning:** List the subdomains you think matter most and why. Which ones should be tested more frequently (weighted higher)?

---

**Step 4: Derive property candidates**

Now, **think through** what properties to test. Optionally frame requirements in quantifier form (for all / for some / for no) and define the domain D; see [propertyderivation.md](propertyderivation.md) § The Quantifier View of Properties. Start with correctness and invariants, then consider:

- **If stateful**: Fundamental properties like crash tolerance, exception-safety, memory safety, observability, consistency
- **If refactoring**: Reference implementation comparison, performance criteria
- **If applicable**: Advanced properties (concurrency, distributed systems, etc.)

For each property candidate you identify:

1. **Name it** (e.g., "sorted output", "crash tolerance")
2. **State the property** in one sentence (what must hold)
3. **Note the source** (where it came from)
4. **Decide if it's stateless or stateful**
5. **Assign priority** (High/Medium/Low)

**Show your reasoning:** For each property, briefly explain why it matters and how you'd test it.

---

**Step 5: Design generators**

For each property, think about:

- **What to generate**: What values or structures? (e.g., "vector<int> size 0..100", "action sequence: push/pop/clear")
- **Weighted subdomains**: Should edge cases be boosted? (e.g., "20% empty, 30% single element, 50% normal")

**Show your reasoning:** Explain your generator choices and why you weighted subdomains the way you did (or why uniform is fine).

---

**Step 6: Prioritize**

Finally, **think about implementation order**: Which properties should be implemented first, second, etc., and why?

**Show your reasoning:** Explain your prioritization.

---

## Output Format

After working through the steps above, produce a **summary** in this format:

```text
## Property Test Candidates

### [Property Name 1]
- **Property**: [One-sentence statement]
- **Source**: [Where it comes from]
- **Kind**: Stateless | Stateful
- **Generators**: [What to generate, weighted subdomains]
- **Priority**: High | Medium | Low
- **Why**: [Brief reason why this matters]

### [Property Name 2]
[... repeat for each property ...]

## Implementation Order
1. [First property] - [why first]
2. [Second property] - [why second]
...
```

---

**Remember:** The goal is to show your reasoning process, not just produce a list. Think through each step, explain your choices, and derive properties systematically.
