# COP 4600 Spring 2015 Shell Project

Khaled Hassan and Mason Turner

Features NOT yet implemented
============================
* I/O redirection (to and from files)
* Built-in commands:
    * `setenv`
    * `printenv`
    * `unsetenv`
* Other commands (not built-in)
* Environment variable expansion
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

