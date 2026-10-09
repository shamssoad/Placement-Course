# Volume 12 — Aptitude, interviews, portfolio, and AI-assisted practice

[Contents](../TEXTBOOK.md) · [Introductory module](../12-placement-practice.md)

## Chapter 1. Ratios, percentages, and weighted quantities

Percentages are multipliers applied to a base. Increasing by p% multiplies by `1+p/100`; decreasing by q% multiplies by `1-q/100`. Equal increase and decrease do not cancel because the second base changed. Their product is `1-(p/100)^2`, a loss for nonzero p.

Profit percentage normally uses cost as the base; discount uses marked price. If cost is 800 and selling price 1000, profit is 200/800=25%, not 20%. If a marked price 1200 is discounted 25%, selling price is 900. State the base before calculating.

Weighted averages combine totals, not just reported averages. If 20 students average 70 and 30 average 80, combined average is `(20*70+30*80)/50=76`. Averaging 70 and 80 gives 75 only when the groups have equal sizes.

## Chapter 2. Rates, work, and mixtures

Rate times time equals amount. A person finishing a job in 12 days contributes 1/12 job per day. Another taking 18 contributes 1/18. Together, under constant independent rates, they take `1/(1/12+1/18)=36/5` days.

If the first works alone for three days, one quarter is complete. The remaining three quarters take `(3/4)/(5/36)=27/5` days together, giving total 42/5 days. Tracking completed work avoids adding times directly.

For mixtures, conserve the relevant quantity. Mixing 2 liters at 20% concentration with 3 liters at 40% gives `(2*.2+3*.4)/5=32%`. Replacing some well-mixed liquid requires updating both removed solute and remaining volume.

Equal-distance average speed is harmonic: distances d at speeds u and v take `d/u+d/v`, so average is `2uv/(u+v)`. Equal-time travel instead gives arithmetic mean. The distinction is a modeling question, not a formula-selection trick.

## Chapter 3. Counting and probability

Use the multiplication rule for sequential independent choices in a counting construction. For arrangements without repetition, order matters; for committees, it does not. Explain why division by r! removes repeated orderings rather than applying a memorized symbol.

Conditional probability restricts the sample space. For two fair dice, given that the sum is seven, six ordered outcomes remain. Exactly one die showing six occurs in two, so the conditional probability is 1/3. Do not use the original 36-outcome denominator after conditioning.

Independence means the joint probability factors; mutually exclusive positive-probability events cannot also be independent. Sampling without replacement generally changes subsequent probabilities. Complement counting can simplify “at least one” questions: one minus probability of none.

## Chapter 4. Logical reasoning and data interpretation

Translate statements carefully. “If A then B” does not imply “If B then A.” Its contrapositive “If not B then not A” is equivalent. For arrangements, track constraints in a table and separate deductions from trial assumptions. A contradiction rejects the assumption branch, not the entire problem.

Read chart axes, units, and denominators. A growing total with a falling percentage can still produce a growing absolute count. Distinguish percentage-point change from percentage change: moving from 20% to 25% is five percentage points and a 25% relative increase.

## Chapter 5. Coding interview communication

Use a short repeatable structure: clarify material ambiguities, build examples, propose a baseline, derive improvement, implement, and test. The interviewer should hear why a discarded candidate is impossible, why a DP state is sufficient, or why an algorithm's assumptions hold.

When a bug appears, trace the first incorrect state. Randomly changing `<` to `<=` until examples pass is fragile. State the interval convention and derive the comparison again. If stuck, explain the precise unresolved issue rather than pretending certainty.

For a 45-minute mock, budget initial understanding and derivation but adapt to the problem. A working simpler solution plus a clear path to improvement can be more useful evidence than an unfinished sophisticated method. Afterward, record the reasoning failure, not merely the problem title.

## Chapter 6. Core CS and design interviews

An answer should connect mechanism to consequence. “An index makes queries faster” is incomplete; explain access pattern, selectivity, write cost, and why a scan might still win. “Mutex prevents races” needs the protected invariant and confirmation that all relevant accesses follow the same protocol.

In design rounds, define scope before classes. Walk through one success and one failure sequence. If a requirement changes, identify the smallest responsibility that should change and explain why. Discuss tradeoffs instead of presenting every familiar pattern as necessary.

## Chapter 7. Portfolio and behavioral evidence

A project description should identify the user problem, implemented behavior, constraints, and evidence. State actual measurements with workload and environment. If no measurement was made, say so; invented performance claims cannot withstand follow-up questions.

For behavioral answers, use a specific situation, your responsibility, your action, and the result. Include a lesson that follows from the event. Avoid making teammates anonymous villains; explain disagreements and evidence fairly. Distinguish your contribution from the team's combined work.

A useful project README includes build/run instructions, examples, architecture, tests, and known limitations. It should let another person reproduce the demonstration without relying on your memory.

## Chapter 8. AI-assisted learning without losing independence

Write the contract yourself before asking for generated code. Request examples, counterexamples, concise reasoning, and testable claims. Verify language/library details against primary documentation when they are uncertain. Long confident explanations can still contain errors.

Use an assistant to critique an attempted solution, generate edge cases, or explain a compiler message. Then implement the corrected method independently and solve a changed version later. If you cannot explain a generated line's role or lifetime behavior, the lesson is not complete.

For a documentation retrieval assistant, separate retrieval quality from answer quality. Did the system retrieve the needed passage? Did the answer stay within its evidence? Did it identify absent information? Evaluate answerable, unanswerable, conflicting, and outdated questions. Retrieved text is data and can contain misleading instructions; it should not control the assistant's operating rules.

## Chapter 9. A sustainable revision system

Classify mistakes as misunderstanding, wrong model, invalid assumption, implementation bug, numeric/lifetime error, or communication gap. Assign a targeted repair. Redo the original after a short delay, then solve a variant after a longer interval with the pattern label hidden.

Use mock scores diagnostically. A high total should not conceal a central correctness failure. Progress when you can derive, implement, test, and explain under new inputs. Problem count measures exposure, not mastery.

## Practice set with worked answers

1. Price rises 30%, then falls 10%. **Answer:** 1.3*.9=1.17, a 17% rise.
2. Two equal distances at 40 and 60 km/h. **Answer:** 2*40*60/100=48 km/h.
3. At least one head in three fair independent coin tosses. **Answer:** 1-(1/2)^3=7/8.
4. All A are B; no B are C. Can any A be C? **Answer:** No; every A lies in B, which is disjoint from C.
5. A query improved from 200ms to 120ms. **Answer:** Latency reduced 40%; throughput cannot be inferred directly without workload/concurrency assumptions.
6. AI proposes a hash-only exact substring comparison. **Answer:** Ask for collision handling; hash equality alone does not establish deterministic exactness.

**Practical gate:** Complete one timed coding mock, one SQL exercise, one core-CS discussion, one project walkthrough, and one behavioral answer. Review recordings or written transcripts for unsupported claims and missing reasoning.
