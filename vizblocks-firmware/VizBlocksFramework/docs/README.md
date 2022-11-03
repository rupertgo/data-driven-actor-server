# Doxygen instructions

**A quick run down of using doxygen**

## Intro

Basically Doxygen is going to look at every `.cpp` and `.h` file and look out for specifically formatted comments.

The full documentation can be [found here](http://www.doxygen.nl/manual/docblocks.html), but here is enough to get started. A Doxyfile has already been generated and options can be tweaked as necessary.

### Example

See the example output by going to `docs/html/index.html` in this repository.

### Install

#### Windows

You can install the binary [following these instruction](http://www.doxygen.nl/manual/install.html#install_bin_windows)

#### macOS

```
brew install doxygen
```

#### Linux Ubuntu / Debian

```
sudo apt-get install doxygen
```

### Comment blocks

Comment blocks can take a number of forms but I suggest using Javadoc c-style comments

> ... starting with two \*'s, like this:
>
> ```cpp
> /**
> * ... text ...
> */
> ```


#### Commands

Commands basically allow for comments to broken down into sections. They can start with either `\` or `@`.
There are a ton of different commands, though all the is needed is just a `\brief` tag at most. [A full reference can be found here](http://www.doxygen.nl/manual/commands.html). You don't need to be beholden to these commands, [you can use your own custom commands if you like](http://www.doxygen.nl/manual/custcmd.html).

A boiler plate commented file would look something like

```cpp
/** \file filename.h
    \brief A Documented file.

    filename.h MUST be the same name as the actual file
*/

/** \class Test
    \brief A test class.

    A more detailed class description.
*/
class Test{};

/**
  \fn void example(int arg1);
  \brief an example function
  \param arg1 the first argument
 */
void example(int arg1);
```

The `doxygen_test.h` is provided as an example.


### Running Doxygen

run in the command line as

```sh
cd path/to/repo/
doxygen Doxyfile
```

### Markdown

[Doxygen supports Markdown!](http://www.doxygen.nl/manual/markdown.html) So you can use markdown in comment blocks if necessary.

In the doxygen file you can also point the frontpage to a `.md` file.

```make
USE_MDFILE_AS_MAINPAGE =
```

This will populate the mainpage with html from the specified markdown file.

### Custom Output


You can specify a html header, footer and custom css files. The easiest approach would be to generate these first

```sh
doxygen -w html header.html footer.html customdoxygen.css
```
then edit them as necessary.

Change these options in the Doxyfile

```make
HTML_HEADER = header.html
HTML_FOOTER = footer.html
HTML_EXTRA_STYLESHEET = my_customdoxygen.css
```
