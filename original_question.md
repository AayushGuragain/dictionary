# Assignment One: MTG Card Data (Searching)

For this assignment, you are to build off of the previous assignment's program which _should_ 
make this much easier to handle.

Create **two** programs, one called `parser` as before that takes the `.csv` file and outputs 
a **sorted index** of entries to a **binary file** (`index.bin`) and the entries themselves 
to a separate binary file. The other program is called `search` and will open the index file 
to binary search a desired card by name repeatedly until the user quits. The card information 
is read from the separate entries file (`cards.bin`) directly.

### Parser

1. As before, parse each line in the file into a `CARD` struct. See the included header file 
for a definition.
1. Once the entire dataset is in memory, sort them by card name using your favorite sorting 
method. Call a C library sort function if you wish (`qsort()` is a good one).
1. In one binary file (`index.bin`), write the card name and **offset in which the record begins** 
in the _other_ file.
1. In the second binary file (`cards.bin`), write _the rest_ of the card data, which should 
be everything but the name.

### Search

1. Parse the entire index file (`index.bin`). Every entry should be placed into a struct and 
into an array.
1. It should already be sorted. Read user input and perform a binary search for that card name.
 **Write your own binary search algorithm for this**.
1. If a match is found, read the byte offset that is associated with that card name. Read 
from the other file (`cards.bin`) to obtain the rest of the card data and assemble the 
`CARD` record. Print it to the screen.

### Sample Interaction

A potential sample interaction between a user and the `search` program:

```
> Sorin, Vampire Lord

Sorin, Vampire Lord                      {4}{B}{B}
Legendary Planeswalker - Sorin              Mythic
--------------------------------------------------
[+1]: Up to one target creature gets +2/+0 until end of turn.
[-2]: Sorin, Vampire Lord deals 4 damage to any target. You gain 4 life.
[-8]: Until end of turn, each Vampire you control gains "{T}: Gain control of target creature."
--------------------------------------------------
                                                 4

> Kaya's Wrath

Kaya's Wrath                          {W}{W}{B}{B}
Sorcery                                       Rare
--------------------------------------------------
Destroy all creatures. You gain life equal to the number of creatures you controlled that were destroyed this way.
--------------------------------------------------

> Lotus Field

Lotus Field                                       
Land                                          Rare
--------------------------------------------------
Hexproof
Lotus Field enters the battlefield tapped.
When Lotus Field enters the battlefield, sacrifice two lands.
{T}: Add three mana of any one color.
--------------------------------------------------

> Peeping Geordinscupe

No result for "Peeping Geordinscupe".

> q

```

> Typing `q` at the prompt and pressing enter quits the `search` program.

### Partial Matching

It seems like partial matching could be an interesting problem to tackle (possibly quite easy).
For example, perhaps the user is shown the same card as above when searching for just "Kaya" 
 well. This could be worth some extra credit. 

### Makefile

Remember, you _must_ write your own `Makefile`, such that when your repository is cloned, 
one only has to run `make` in the folder to produce the `parser` and `search` applications. 
**Also write a `clean` target** that removes binaries, `.o` object files and output files, 
so that we may make your program from scratch easily.
