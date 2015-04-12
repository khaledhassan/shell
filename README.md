# COP 4600 Spring 2015 Shell Project

Khaled Hassan and Mason Turner

How to run
==========
`cd src`
`make`
`./shell`

Features NOT yet implemented
============================
* Environment variable expansion
* Alias expansion
* Wildcard matching
* Extra credit: Tilde expansion
* Extra credit: File name completion

Features implemented
====================
* Built-in commands:
    * `cd`
        * `cd` (with no arguments) - go `HOME`
        * `cd` `some path` - go to `some path`
    * `alias`
        * `alias` (with no arguments) - print list of aliases 
        * `alias name word` - set new alias
        * `unalias name` - remove alias 
    * `bye` (or `EOF`)
    * `setenv`
    * `printenv`
    * `unsetenv`
* Other commands (not built-in)
* I/O redirection
    * from command to command
    * to and from files
    * stderr to stdout (some testing done, more would be nice)
    * stderr to file (some testing done, more would be nice)


