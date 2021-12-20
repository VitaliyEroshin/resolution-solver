# ResolutionSolver
## Danger!
This code may be harmful for your computer. There may be a lot of bugs.
## Idea
There are 3 basic structures: Literal, Disjunct and CNF. You can get Literal from stdin, then pass it to Disjunct (```Disjunct.push(Literal)```),
and CNF is made up from Disjunct (```CNF.push(Disjunct)```. ```CNF.process()``` produces new disjuncts from existing. You can also pass verbose parameter,
like so: ```CNF.process(verbose=true)```. Then it will show it's state after iteration. You can also output you CNF, Disjunct or Literal to stdout.
