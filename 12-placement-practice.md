# Module 12 — Aptitude, interviews, and AI-assisted practice

**Expanded textbook:** [Aptitude, interviews, portfolio, and AI practice](textbook/12-placement.md). This file is the introductory/revision version.

Prerequisite: take aptitude early; take technical mocks after the relevant modules. Outcome: demonstrate ability under realistic constraints.

## Lesson 12.1 — Percentages, ratios, and averages

Percent changes act on the current base. A 20% increase followed by a 20% decrease multiplies by `1.2 * 0.8 = 0.96`, leaving a 4% loss. Adding the percentages as if the base were unchanged is incorrect.

A ratio 3:2 divides a total into five equal parts. For a total of 250, the shares are 150 and 100. A weighted average uses the quantities as weights: 2 kg costing 30 per kg mixed with 3 kg costing 50 per kg gives `(2*30 + 3*50)/5 = 42` per kg.

**Exercises:** (a) A price rises 25%, then falls 20%. (b) A group has an average of 18 across five people; a sixth person aged 30 joins. **Answers:** (a) Original price, since `1.25*0.8=1`. (b) `(5*18+30)/6=20`.

## Lesson 12.2 — Work, speed, and counting

Treat work as one complete job. Someone finishing in six days works at 1/6 job per day; another finishing in three works at 1/3. Together they work at 1/2, so completion takes two days, assuming rates are constant and independent.

Relative speed adds when two objects approach each other and subtracts when one catches another moving in the same direction. Always convert units before dividing distance by speed. Average speed over equal distances is generally not the arithmetic mean of the speeds: compute total distance divided by total time.

The multiplication principle counts sequential choices. Use permutations when order matters and combinations when it does not. For probability, define the sample space and whether outcomes are equally likely.

**Exercise:** Travel equal distances at 30 and 60 km/h. **Answer:** Average speed is 40 km/h; choose 60 km per leg to compute 120 km in three hours.

## Lesson 12.3 — Logical reasoning and reading

Translate constraints into a table or diagram rather than juggling them mentally. In an arrangement puzzle, place the most restrictive condition first, propagate consequences, and branch only when necessary. Label deductions and assumptions differently.

“All A are B” does not imply “All B are A.” “Some A are B” establishes overlap but does not identify every A. A conclusion needs to follow in all models satisfying the premises; one counterexample disproves necessity.

For reading comprehension, distinguish the author's claim, evidence, and qualification. Do not choose an answer merely because it is true in the real world; it must follow from the passage.

**Exercise:** All developers are employees; some employees are managers. Must some developers be managers? **Answer:** No. Managers could all be outside the developer subset.

## Lesson 12.4 — A coding interview is a reasoning conversation

Use this sequence: clarify, model, baseline, improve, implement, test, summarize. Ask about empty input, bounds, duplicates, and required output only where they change the solution. Do not spend the whole interview requesting details you can state as reasonable assumptions.

Example prompt: “Count subarrays summing to k.” First describe an O(n²) scan. Recognize each sum as the difference of prefixes. Derive the need to store earlier prefix frequencies. Explain the initial zero prefix and lookup-before-insertion rule. Then implement and test `[0,0]` with k=0 and an array containing negatives.

When stuck, name the obstacle: “This state does not remember whether the item was used.” That is more useful than silently trying syntax changes. A hint should trigger revised reasoning, not an unexplained jump to a memorized solution.

## Lesson 12.5 — Project and behavioral discussion

A strong project explanation begins with the user's problem and the resulting behavior. Then describe the architecture, one difficult tradeoff, evidence from testing, and a limitation. Separate what you implemented from what you would add later.

Use a concrete situation, your responsibility, your action, and the result for behavioral stories. “We worked hard” hides your contribution. “I reduced the failing input to a two-step reservation race and added a database uniqueness constraint” provides evidence.

For resume bullets, state what was built and what changed. Include measurements only when you actually measured them and can explain the setup. Do not invent traffic, performance, or users for a classroom project.

**Assignment:** Record a three-minute project walkthrough. Review whether each claim is supported by code, tests, or an honest stated limitation.

## Lesson 12.6 — Use AI as a checked collaborator

Before asking for code, write a contract: inputs, outputs, constraints, examples, and failure behavior. A useful prompt for this course is:

> Review this C++17 function against the attached contract. Identify a concrete failing input for each suspected issue. Explain correctness and complexity. Do not rewrite it until the issues are established.

Check every proposed bug and fix yourself. Generated code can compile and still violate ownership or algorithm assumptions. Use brute-force comparisons and adversarial examples. During initial practice, try the problem independently before consulting an assistant, then redo it without the assistant.

Ask for concise reasoning, assumptions, and checkable evidence; a long explanation alone does not prove correctness. Do not share credentials or confidential source material with a tool without appropriate authorization.

## Lesson 12.7 — Optional retrieval assistant lab

A retrieval-assisted system first finds relevant passages, then supplies them as context for an answer. Embeddings map texts to numeric vectors intended to capture similarity; nearest-neighbor retrieval is not a truth test.

Build a small assistant over your project documentation. Store passages with file and section IDs, retrieve a few for a question, and require the answer to cite the actual provided passages. Use a C++ client if calling an inference service; the service can handle model execution. Keep credentials outside source control.

Evaluate with answerable questions, questions whose answers are absent, conflicting passages, and stale documentation. Score whether cited passages actually support the answer. Retrieved text is data, not authority to change program instructions. A relevant-looking passage can contain malicious or irrelevant instructions.

## Mock routine and marking

For a 45-minute coding mock, allow roughly 5 minutes for clarification, 10 for derivation, 20 for implementation, and 10 for testing and explanation. Adapt rather than watching the clock rigidly.

Score 0–4 each for modeling, correctness, efficiency, implementation, and communication. A polished explanation cannot compensate for an incorrect algorithm. Record the actual failure and schedule a changed version for revision.

**Exit ticket:** Complete one aptitude set, one coding mock, one core CS discussion, and a project walkthrough. Explain one answer that AI got wrong and how you established the correction.
