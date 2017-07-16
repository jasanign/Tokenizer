# Tokenizer

This project is a lexer (that is, a tokenizer), written in the C programming language, that parses lines made up of simple math and logical expressions and break them down into meaningful tokens. For example, if I had the line 12=17, i=this program outputs "12", "=", and "17".

This program is capable of recognizing the following lexemes:

	+
	-
	*
	/
	(
	)
	^
	=
	<
	<=
	>
	>=
	==
	!
	!=
	;
	[0-9]+ -> an integer literal (that is, one or more occurrences of any digit)

Example outputs are in output files.
