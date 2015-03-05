/*!
	\mainpage Overview

	\tableofcontents

	\section introduction Introduction

	`utf8rewind` is a C library designed to extend default string handling
	functions in order to add support for UTF-8 encoded text. Besides providing
	functions to deal with UTF-8 encoded text, it also provides functions for
	converting to and from UTF-16 encoded text, the default on Windows.

	For a full summary of the interface, please refer to the
	[library interface](\ref utf8rewind.h).

	\section why-utf8 Why UTF-8?

	UTF-8 encoded Unicode accounts for[over 60 percent of the web]
	(http://googleblog.blogspot.nl/2012/02/unicode-over-60-percent-of-web.html).
	And with good reason! Because UTF-8 is completely backwards-compatible with
	ASCII, developers only need to change code dealing with codepoints. UTF-8
	can encode the full range of Unicode codepoints in a maximum of four bytes
	per codepoint. However, because most text tends to be the Latin alphabet
	mixed with special characters, the common case is strings not much longer
	than pure ASCII.

	\section why-not-utf16 Why not UTF-16?

	UTF-16 encoding solves the same problems as UTF-8, but in a different way.
	UTF-16 is not backwards-compatible with ASCII, resulting in invalid
	codepoints being encountered when the string is treated as ASCII. As a
	result, all code dealing with strings must be changed in order to handle
	these new strings. This can be seen in the changes made in the C strings
	API:

	Description                | ASCII   | UTF-16
	-------------------------- | ------- | ------
	Get the length of a string | strlen  | wcslen
	Copy a string to another   | strcpy  | wcscpy
	Append to a string         | strcat  | wcscat
	Convert to lowercase       | tolower | towlower

	Converting a project to use UTF-16 after the fact is a serious endeavour
	that touches *all* code dealing with strings. On the other hand,
	changing existing code to use UTF-8 only deals with codepoint processing.

	\section license Licensing

	This project is licensed under the MIT license, a full copy of which
	should have been provided with the project.

	\section building Building the project

	All supported platforms use [GYP](http://code.google.com/p/gyp/) to
	generate a solution. This generated solution can be used to compile the
	project and its dependencies.

	\subsection building-windows Building on Windows with Visual Studio

	\note You will need to have Visual Studio 2010 or above installed.

	Open a command window at the project's root.

	If you have multiple versions of Visual Studio installed, you must first
	specify the version you want to use:

		set GYP_MSVS_VERSION=2012

	Use GYP to generate a solution:

		tools\gyp\gyp --depth utf8rewind.gyp

	Open the solution in Visual Studio. You can use the different build
	configurations to generate a static library.

	\subsection building-linux Building on Linux with GCC

	Open a command window at the project's root.

	Ensure you have all dependencies installed using your favorite preferred
	package manager:

		sudo apt-get install gyp gcc g++

	Use GYP to generate a soluation:

		gyp --depth= utf8rewind.gyp

	Build the project using `make`:

		make

	For a release build, specify the build type:

		make BUILDTYPE=Release

	\subsection building-mac Building on Mac OS X using Xcode

	\note Building on Mac OS X is currently untested. Please let us know if
	you can help us in this regard.

	Open a command window at the project's root.

	Use GYP to generate a solution:

		tools\gyp\gyp --depth utf8rewind.gyp

	Open the solution in Xcode and you can build the library and tests.

	\subsection running-tests Running the tests

	After generating a solution, build and run the "tests-rewind" project.
	Verify that all tests pass on your system configuration before continuing.

	\section helping-out Helping out

	As a user, you can help the project in a number of ways, in order of
	difficulty:

	* **Use it** - Designers of a public interface often have very different
	ideas about usability than those actually using it. By using the library,
	you are helping the project spread and could potentially improve it by us
	taking your project into consideration when we design the API.

	* **Spread the word** - If you find `utf8rewind` useful, recommend it to
	your friends and coworkers.

	* **Complain** - No library is perfect and `utf8rewind` is no exception.
	If you find a fault but lack the means (time, resources, etc.) to fix it,
	sending complaints to the proper channels can help the project out a lot.

	* **Write a failing test** - If a feature is not working as intended, you
	can prove it by writing a failing test. By sending the test to us, we can
	make the adjustments necessary for it to pass.

	* **Write a patch** - Patches include a code change that help tests to
	pass. A patch must always include a set of tests that fail to pass without
	the patch. All patches will be reviewed and possibly cleaned up before
	being accepted.

	\section contact Contact

	For inquiries, complaints and patches, please contact `{quinten}{lansu} {at} {gmail}.{com}`. Remove the brackets to get a valid e-mail address.
*/