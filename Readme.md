# Language specification

## Objects

**String**

* “text text text”

**Number**

* 42.3
* 42

**Bool**

* yes
* no

**Void**

* void

**Eof**

* eof

**List**

* [ 1 2 3 “a” [ 5 6 ] yes no 42 ]

## Basic operations

**Compare**

* == 5 3
* == “a” “b”
* == eof “eof”
* == [ 1 2 3 ] [ 1 2 3 ]
* >= 1 2
* <= 1 2
* < 1 2
* > 1 2

**Assign**

* = b 1
* += b 2
* -= b 3
* *= b 4
* /= b 5
* = a b

**Arithmetic**

* + 1 2
* - 1 2
* * 1 2
* / 1 2

**Logic**

* and yes no
* or yes no
* not yes


**Append**

* + “abcd” “e”
* + [ “a” “b” “c” 1 2 3 ] “e”
* + “abcde” “efgh”
* + [ “a” “b” “c” 1 2 3 ] [ 5 6 7 “g” ]

**Size**

* size “abc”
* size [ 1 2 “abc” ]

**At**

* at “abc” 2
* at [ 1 2 “abc” ] 1

## I/O

**Print - any object**

* print “abc”

**Open file**

* = f open “/path/file.txt”

**Write**

* write f “text text text”
* write open “/path/file.txt” “text text text”

**Read line**

* read f

## Control flow

**Return**

* return 5
* return void

**If-Else**

* if == 10 20 ( … )
* if yes ( print “a” ) else ( ... )

**Loop**

* loop ( … )

**End Loop**

* loop ( … if == 1 1 ( end ) )

## User functions

**Function def - last call of block is result**

* func func_name arg1_id … arnN_id ( … )

**Function call**

* func_name arg1 … argN
