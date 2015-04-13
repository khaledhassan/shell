# COP 4600 Spring 2015 Shell Project

Khaled Hassan and Mason Turner

How to run
==========
1. `cd src`
2. `make`
3. `./shell`

Features NOT yet implemented
============================
* Wildcard matching
    * other directories/paths (is this even necessary?)
    * single character `?`
* Extra credit: Tilde expansion
    * works when at the beginning of a token; should be ignored in other places, but currently isn't
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
* Wildcard matching
    * current directory only (except for a single `*`)
* Environment variable expansion
* Alias expansion
    * Limitations:
        * ~~Expansion done on any word that matches an alias~~ fixed: only expands on first token
        * No loop detection

