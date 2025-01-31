# Wav file generator

This is a wav file generator that I built as part of my c++ programming course ar Reykjavik University. Using the 98 standard.

The version that was solved was part b.

## Usage

These are the make command for the project

```
  //create the binary files
  make

  // creates the out files
  make release

  // creates the debug target
  make debug

  // cleans removes everything except the source code
  make clean

  // Removes all wav files from the project
  make clean_wav
```

To create a a wav file you can run with the following command line arguments `[file_name]`, `[frequency]`,`[duration]`.

Where :

- `[file_name]` is a string the file extension will be added by the program.
- `[frequency]` is a int,
- `[duration]` is a real number e.g 0.5

```
  release.out [file_name] [frequency] [duration]
```

If no arguments are passed the program will default to its default parameters.

Author: Viktor Hollanders
