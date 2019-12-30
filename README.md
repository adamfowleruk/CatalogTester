# Catalog Tester

This Haiku app tests a set of re-ordered catkeys files for a consistent fingerprint, 
and attempts to fine which line the first mis-match of keys occurs on.

## Why do you need the tool?

It is possible to easily create invalid catkeys files and not have a
problem show up until you try and use linkcatkeys to join translations
in to your final app.

Common problems include:-

- A change of case in any character between catkey versions
- An addition/removal of trailing whitespace in keys between catkey versions
- Non escaped newline (\n) and tab (\t) characters in keys

## How does the tool help?

By recomputing the fingerprint from scratch when loading a catkeys file
using the same algorithm as used in Haiku (uses the exact same library code),
the tool will show the actual, correct current fingerprint of each catkeys file.

The tool also assumes the first catkeys file supplied is generated from your app.
This is usually the default English language file. It uses this file to cache
the fingerprint and key after each line in the source file. This enables the
tool to show you exactly where your file diverged from the source language.

Naturally, as this is a hash algorithm, it can only easily spot the first key
this diverges on.

WARNING: You must reorder your catkeys files to ensure all keys are in the same
order. Read (but DO NOT run) the reorder.sh file in this repository for a command
that will reorder a catkeys file.

## Examples

The default Paladin project file will launch the tool using the provided three
example ordered catkeys files. Source is English, correct matching file is German,
incorrect mismatch file is Italian.

## License

This code is licensed under the MIT license. See the LICENSE file for details.
