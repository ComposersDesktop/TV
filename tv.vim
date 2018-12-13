" Vim syntax file
" Language:	Tabula Vigilans
" Maintainer:	John ffitch
" Last Change:	2014 Jun 19

" Quit when a (custom) syntax file was already loaded
if exists("b:current_syntax")
  finish
endif

let s:cpo_save = &cpo
set cpo&vim

let s:ft = matchstr(&ft, '^\([^.]\)\+')

" A bunch of useful tv keywords
syn keyword	tvStatement	goto break return continue
syn keyword	tvLabel		case default
syn keyword	tvConditional	if else switch
syn keyword	tvRepeat		while for do

" String and Character constants
" Highlight special characters (those which have a backslash) differently
syn match	tvSpecial	display contained "\\\(x\x\+\|\o\{1,3}\|.\|$\)"
if !exists("c_no_utf")
  syn match	tvSpecial	display contained "\\\(u\x\{4}\|U\x\{8}\)"
endif
if exists("c_no_cformat")
  syn region	tvString		start=+L\="+ skip=+\\\\\|\\"+ end=+"+ contains=cSpecial,@Spell extend
  " tvCppString: same as tvString, but ends at end of line
  if !exists("cpp_no_cpp11") " ISO C++11
    syn region tvCppString	start=+\(L\|u\|u8\|U\|R\|LR\|u8R\|uR\|UR\)\="+ skip=+\\\\\|\\"\|\\$+ excludenl end=+"+ end='$' contains=cSpecial,cFormat,@Spell
  else
    syn region tvCppString	start=+L\="+ skip=+\\\\\|\\"\|\\$+ excludenl end=+"+ end='$' contains=cSpecial,cFormat,@Spell
  endif
  syn region	tvCppOut2	contained start="0" end="^\s*\(%:\|#\)\s*\(endif\>\|else\>\|elif\>\)" contains=cSpaceError,cCppSkip
  syn region	tvCppSkip	contained start="^\s*\(%:\|#\)\s*\(if\>\|ifdef\>\|ifndef\>\)" skip="\\$" end="^\s*\(%:\|#\)\s*endif\>" contains=cSpaceError,cCppSkip
else
    syn match	tvFormat		display "%\(\d\+\$\)\=[-+' #0*]*\(\d*\|\*\|\*\d\+\$\)\(\.\(\d*\|\*\|\*\d\+\$\)\)\=\([hlL]\|ll\)\=\([bdiuoxXDOUfeEgGcCsSpn]\|\[\^\=.[^]]*\]\)" contained
  syn match	tvFormat		display "%%" contained
  syn region	tvString		start=+L\="+ skip=+\\\\\|\\"+ end=+"+ contains=cSpecial,cFormat,@Spell extend
  " tvCppString: same as tvString, but ends at end of line
  syn region	tvCppString	start=+L\="+ skip=+\\\\\|\\"\|\\$+ excludenl end=+"+ end='$' contains=cSpecial,cFormat,@Spell
endif

syn match	tvCharacter	"L\='[^\\]'"
syn match	tvCharacter	"L'[^']*'" contains=cSpecial
syn match	tvSpecialError	"L\='\\[^'\"?\\abfnrtv]'"
syn match	tvSpecialCharacter "L\='\\['\"?\\abfnrtv]'"
syn match	tvSpecialCharacter display "L\='\\\o\{1,3}'"
syn match	tvSpecialCharacter display "'\\x\x\{1,2}'"
syn match	tvSpecialCharacter display "L'\\x\x\+'"

"when wanted, highlight trailing white space
if exists("c_space_errors")
  if !exists("c_no_trail_space_error")
    syn match	tvSpaceError	display excludenl "\s\+$"
  endif
  if !exists("c_no_tab_space_error")
    syn match	tvSpaceError	display " \+\t"me=e-1
  endif
endif

" This should be before tvErrInParen to avoid problems with #define ({ xxx })
if exists("c_curly_error")
  syn match tvCurlyError "}"
  syn region	tvBlock		start="{" end="}" contains=ALLBUT,cBadBlock,cCurlyError,@cParenGroup,cErrInParen,cCppParen,cErrInBracket,cCppBracket,cCppString,@Spell fold
else
  syn region	tvBlock		start="{" end="}" transparent fold
endif

"catch errors caused by wrong parenthesis and brackets
" also accept <% for {, %> for }, <: for [ and :> for ] (C99)
" But avoid matching <::.
syn cluster	tvParenGroup	contains=cParenError,cIncluded,cSpecial,cCommentSkip,cCommentString,cComment2String,@cCommentGroup,cCommentStartError,cUserLabel,cOctalZero,@cCppOutInGroup,cFormat,cNumber,cFloat,cOctal,cOctalError,cNumbersCom
syn region	tvParen		transparent start='(' end=')' end='}'me=s-1 contains=ALLBUT,cBlock,@cParenGroup,cCppParen,cCppString,@Spell
    " tvCppParen: same as tvParen but ends at end-of-line; used in tvDefine
    syn region	tvCppParen	transparent start='(' skip='\\$' excludenl end=')' end='$' contained contains=ALLBUT,@cParenGroup,cParen,cString,@Spell
    syn match	tvParenError	display ")"
    syn match	tvErrInParen	display contained "^[{}]\|^<%\|^%>"
    
elseif exists("c_no_bracket_error")
  if s:ft ==# 'cpp' && !exists("cpp_no_cpp11")
    syn region	tvParen		transparent start='(' end=')' contains=ALLBUT,@cParenGroup,cCppParen,cCppString,@Spell
    " tvCppParen: same as tvParen but ends at end-of-line; used in tvDefine
    syn region	tvCppParen	transparent start='(' skip='\\$' excludenl end=')' end='$' contained contains=ALLBUT,@cParenGroup,cParen,cString,@Spell
    syn match	tvParenError	display ")"
    syn match	tvErrInParen	display contained "<%\|%>"
  else
    syn region	tvParen		transparent start='(' end=')' end='}'me=s-1 contains=ALLBUT,cBlock,@cParenGroup,cCppParen,cCppString,@Spell
    " tvCppParen: same as tvParen but ends at end-of-line; used in tvDefine
    syn region	tvCppParen	transparent start='(' skip='\\$' excludenl end=')' end='$' contained contains=ALLBUT,@cParenGroup,cParen,cString,@Spell
    syn match	tvParenError	display ")"
    syn match	tvErrInParen	display contained "[{}]\|<%\|%>"
  endif
else
  if s:ft ==# 'cpp' && !exists("cpp_no_cpp11")
    syn region	tvParen		transparent start='(' end=')' contains=ALLBUT,@cParenGroup,cCppParen,cErrInBracket,cCppBracket,cCppString,@Spell
    " tvCppParen: same as tvParen but ends at end-of-line; used in tvDefine
    syn region	tvCppParen	transparent start='(' skip='\\$' excludenl end=')' end='$' contained contains=ALLBUT,@cParenGroup,cErrInBracket,cParen,cBracket,cString,@Spell
    syn match	tvParenError	display "[\])]"
    syn match	tvErrInParen	display contained "<%\|%>"
    syn region	tvBracket	transparent start='\[\|<::\@!' end=']\|:>' contains=ALLBUT,@cParenGroup,cErrInParen,cCppParen,cCppBracket,cCppString,@Spell
  else
    syn region	tvParen		transparent start='(' end=')' end='}'me=s-1 contains=ALLBUT,cBlock,@cParenGroup,cCppParen,cErrInBracket,cCppBracket,cCppString,@Spell
    " tvCppParen: same as tvParen but ends at end-of-line; used in tvDefine
    syn region	tvCppParen	transparent start='(' skip='\\$' excludenl end=')' end='$' contained contains=ALLBUT,@cParenGroup,cErrInBracket,cParen,cBracket,cString,@Spell
    syn match	tvParenError	display "[\])]"
    syn match	tvErrInParen	display contained "[\]{}]\|<%\|%>"
    syn region	tvBracket	transparent start='\[\|<::\@!' end=']\|:>' end='}'me=s-1 contains=ALLBUT,cBlock,@cParenGroup,cErrInParen,cCppParen,cCppBracket,cCppString,@Spell
  endif
  " tvCppBracket: same as tvParen but ends at end-of-line; used in tvDefine
  syn region	tvCppBracket	transparent start='\[\|<::\@!' skip='\\$' excludenl end=']\|:>' end='$' contained contains=ALLBUT,@cParenGroup,cErrInParen,cParen,cBracket,cString,@Spell
  syn match	tvErrInBracket	display contained "[);{}]\|<%\|%>"
endif

if s:ft ==# 'c' || exists("cpp_no_cpp11")
  syn region	tvBadBlock	keepend start="{" end="}" contained containedin=cParen,cBracket,cBadBlock transparent fold
endif

"integer number, or floating point number without a dot and with "f".
syn case ignore
syn match	tvNumbers	display transparent "\<\d\|\.\d" contains=cNumber,cFloat,cOctalError,cOctal
" Same, but without octal error (for comments)
syn match	tvNumbersCom	display contained transparent "\<\d\|\.\d" contains=cNumber,cFloat,cOctal
syn match	tvNumber		display contained "\d\+\(u\=l\{0,2}\|ll\=u\)\>"
"hex number
syn match	tvNumber		display contained "0x\x\+\(u\=l\{0,2}\|ll\=u\)\>"
" Flag the first zero of an octal number as something special
syn match	tvOctal		display contained "0\o\+\(u\=l\{0,2}\|ll\=u\)\>" contains=cOctalZero
syn match	tvOctalZero	display contained "\<0"
syn match	tvFloat		display contained "\d\+f"
"floating point number, with dot, optional exponent
syn match	tvFloat		display contained "\d\+\.\d*\(e[-+]\=\d\+\)\=[fl]\="
"floating point number, starting with a dot, optional exponent
syn match	tvFloat		display contained "\.\d\+\(e[-+]\=\d\+\)\=[fl]\=\>"
"floating point number, without dot, with exponent
syn match	tvFloat		display contained "\d\+e[-+]\=\d\+[fl]\=\>"
if !exists("c_no_c99")
  "hexadecimal floating point number, optional leading digits, with dot, with exponent
  syn match	tvFloat		display contained "0x\x*\.\x\+p[-+]\=\d\+[fl]\=\>"
  "hexadecimal floating point number, with leading digits, optional dot, with exponent
  syn match	tvFloat		display contained "0x\x\+\.\=p[-+]\=\d\+[fl]\=\>"
endif

" flag an octal number with wrong digits
syn match	tvOctalError	display contained "0\o*[89]\d*"
syn case match

if exists("c_comment_strings")
  " A comment can contain cString, cCharacter and cNumber.
  " But a "*/" inside a cString in a cComment DOES end the comment!  So we
  " need to use a special type of cString: cCommentString, which also ends on
  " "*/", and sees a "*" at the start of the line as comment again.
  " Unfortunately this doesn't very well work for // type of comments :-(
  syn match	tvCommentSkip	contained "^\s*\*\($\|\s\+\)"
  syn region tvCommentString	contained start=+L\=\\\@<!"+ skip=+\\\\\|\\"+ end=+"+ end=+\*/+me=s-1 contains=cSpecial,cCommentSkip
  syn region tvComment2String	contained start=+L\=\\\@<!"+ skip=+\\\\\|\\"+ end=+"+ end="$" contains=cSpecial
  syn region  tvCommentL	start="//" skip="\\$" end="$" keepend contains=@cCommentGroup,cComment2String,cCharacter,cNumbersCom,cSpaceError,@Spell
  if exists("c_no_comment_fold")
    " Use "extend" here to have preprocessor lines not terminate halfway a
    " comment.
    syn region tvComment	matchgroup=cCommentStart start="/\*" end="\*/" contains=@cCommentGroup,cCommentStartError,cCommentString,cCharacter,cNumbersCom,cSpaceError,@Spell extend
  else
    syn region tvComment	matchgroup=cCommentStart start="/\*" end="\*/" contains=@cCommentGroup,cCommentStartError,cCommentString,cCharacter,cNumbersCom,cSpaceError,@Spell fold extend
  endif
else
  syn region	tvCommentL	start="//" skip="\\$" end="$" keepend contains=@cCommentGroup,cSpaceError,@Spell
  if exists("c_no_comment_fold")
    syn region	tvComment	matchgroup=cCommentStart start="/\*" end="\*/" contains=@cCommentGroup,cCommentStartError,cSpaceError,@Spell extend
  else
    syn region	tvComment	matchgroup=cCommentStart start="/\*" end="\*/" contains=@cCommentGroup,cCommentStartError,cSpaceError,@Spell fold extend
  endif
endif
" keep a // comment separately, it terminates a preproc. conditional
syn match	tvCommentError	display "\*/"
syn match	tvCommentStartError display "/\*"me=e-1 contained

syn keyword	tvOperator	sizeof
if exists("c_gnu")
  syn keyword	tvStatement	__asm__
  syn keyword	tvOperator	typeof __real__ __imag__
endif
syn keyword	tvType		int
syn keyword	tvType		double

syn keyword	tvStructure	struct union enum typedef
syn keyword	tvStorageClass	static extern const

syn keyword tvConstant M_1_PI M_2_PI M_2_SQRTPI M_SQRT2 M_SQRT1_2
syn keyword tvConstant true false

" Accept %: for # (C99)
syn region	tvPreCondit	start="^\s*\(%:\|#\)\s*\(if\|ifdef\|ifndef\|elif\)\>" skip="\\$" end="$" keepend contains=cComment,cCommentL,cCppString,cCharacter,cCppParen,cParenError,cNumbers,cCommentError,cSpaceError
syn match	tvPreConditMatch	display "^\s*\(%:\|#\)\s*\(else\|endif\)\>"
if !exists("c_no_if0")
  syn cluster	tvCppOutInGroup	contains=cCppInIf,cCppInElse,cCppInElse2,cCppOutIf,cCppOutIf2,cCppOutElse,cCppInSkip,cCppOutSkip
  syn region	tvCppOutWrapper	start="^\s*\(%:\|#\)\s*if\s\+0\+\s*\($\|//\|/\*\|&\)" end=".\@=\|$" contains=cCppOutIf,cCppOutElse,@NoSpell fold
  syn region	tvCppOutIf	contained start="0\+" matchgroup=cCppOutWrapper end="^\s*\(%:\|#\)\s*endif\>" contains=cCppOutIf2,cCppOutElse
  if !exists("c_no_if0_fold")
    syn region	tvCppOutIf2	contained matchgroup=cCppOutWrapper start="0\+" end="^\s*\(%:\|#\)\s*\(else\>\|elif\s\+\(0\+\s*\($\|//\|/\*\|&\)\)\@!\|endif\>\)"me=s-1 contains=cSpaceError,cCppOutSkip,@Spell fold
  else
    syn region	tvCppOutIf2	contained matchgroup=cCppOutWrapper start="0\+" end="^\s*\(%:\|#\)\s*\(else\>\|elif\s\+\(0\+\s*\($\|//\|/\*\|&\)\)\@!\|endif\>\)"me=s-1 contains=cSpaceError,cCppOutSkip,@Spell
  endif
  syn region	tvCppOutElse	contained matchgroup=cCppOutWrapper start="^\s*\(%:\|#\)\s*\(else\|elif\)" end="^\s*\(%:\|#\)\s*endif\>"me=s-1 contains=TOP,cPreCondit
  syn region	tvCppInWrapper	start="^\s*\(%:\|#\)\s*if\s\+0*[1-9]\d*\s*\($\|//\|/\*\||\)" end=".\@=\|$" contains=cCppInIf,cCppInElse fold
  syn region	tvCppInIf	contained matchgroup=cCppInWrapper start="\d\+" end="^\s*\(%:\|#\)\s*endif\>" contains=TOP,cPreCondit
  if !exists("c_no_if0_fold")
    syn region	tvCppInElse	contained start="^\s*\(%:\|#\)\s*\(else\>\|elif\s\+\(0*[1-9]\d*\s*\($\|//\|/\*\||\)\)\@!\)" end=".\@=\|$" containedin=cCppInIf contains=cCppInElse2 fold
  else
    syn region	tvCppInElse	contained start="^\s*\(%:\|#\)\s*\(else\>\|elif\s\+\(0*[1-9]\d*\s*\($\|//\|/\*\||\)\)\@!\)" end=".\@=\|$" containedin=cCppInIf contains=cCppInElse2
  endif
  syn region	tvCppInElse2	contained matchgroup=cCppInWrapper start="^\s*\(%:\|#\)\s*\(else\|elif\)\([^/]\|/[^/*]\)*" end="^\s*\(%:\|#\)\s*endif\>"me=s-1 contains=cSpaceError,cCppOutSkip,@Spell
  syn region	tvCppOutSkip	contained start="^\s*\(%:\|#\)\s*\(if\>\|ifdef\>\|ifndef\>\)" skip="\\$" end="^\s*\(%:\|#\)\s*endif\>" contains=cSpaceError,cCppOutSkip
  syn region	tvCppInSkip	contained matchgroup=cCppInWrapper start="^\s*\(%:\|#\)\s*\(if\s\+\(\d\+\s*\($\|//\|/\*\||\|&\)\)\@!\|ifdef\>\|ifndef\>\)" skip="\\$" end="^\s*\(%:\|#\)\s*endif\>" containedin=cCppOutElse,cCppInIf,cCppInSkip contains=TOP,cPreProc
endif
syn region	tvIncluded	display contained start=+"+ skip=+\\\\\|\\"+ end=+"+
syn match	tvIncluded	display contained "<[^>]*>"
syn match	tvInclude	display "^\s*\(%:\|#\)\s*include\>\s*["<]" contains=cIncluded
"syn match tvLineSkip	"\\$"
syn cluster	tvPreProcGroup	contains=cPreCondit,cIncluded,cInclude,cDefine,cErrInParen,cErrInBracket,cUserLabel,cSpecial,cOctalZero,cCppOutWrapper,cCppInWrapper,@cCppOutInGroup,cFormat,cNumber,cFloat,cOctal,cOctalError,cNumbersCom,cString,cCommentSkip,cCommentString,cComment2String,@cCommentGroup,cCommentStartError,cParen,cBracket,cMulti,cBadBlock
syn region	tvDefine		start="^\s*\(%:\|#\)\s*\(define\|undef\)\>" skip="\\$" end="$" keepend contains=ALLBUT,@cPreProcGroup,@Spell
syn region	tvPreProc	start="^\s*\(%:\|#\)\s*\(pragma\>\|line\>\|warning\>\|warn\>\|error\>\)" skip="\\$" end="$" keepend contains=ALLBUT,@cPreProcGroup,@Spell

" Highlight User Labels
syn cluster	tvMultiGroup	contains=cIncluded,cSpecial,cCommentSkip,cCommentString,cComment2String,@cCommentGroup,cCommentStartError,cUserCont,cUserLabel,cBitField,cOctalZero,cCppOutWrapper,cCppInWrapper,@cCppOutInGroup,cFormat,cNumber,cFloat,cOctal,cOctalError,cNumbersCom,cCppParen,cCppBracket,cCppString
if s:ft ==# 'c' || exists("cpp_no_cpp11")
  syn region	tvMulti		transparent start='?' skip='::' end=':' contains=ALLBUT,@cMultiGroup,@Spell
endif
" Avoid matching foo::bar() in C++ by requiring that the next char is not ':'
syn cluster	tvLabelGroup	contains=cUserLabel
syn match	tvUserCont	display "^\s*\I\i*\s*:$" contains=@cLabelGroup
syn match	tvUserCont	display ";\s*\I\i*\s*:$" contains=@cLabelGroup
syn match	tvUserCont	display "^\s*\I\i*\s*:[^:]"me=e-1 contains=@cLabelGroup
syn match	tvUserCont	display ";\s*\I\i*\s*:[^:]"me=e-1 contains=@cLabelGroup

syn match	tvUserLabel	display "\I\i*" contained

if exists("c_minlines")
  let b:c_minlines = c_minlines
else
  if !exists("c_no_if0")
    let b:c_minlines = 50	" #if 0 constructs can be long
  else
    let b:c_minlines = 15	" mostly for () constructs
  endif
endif
if exists("c_curly_error")
  syn sync fromstart
else
  exec "syn sync tvcomment tvComment minlines=" . b:c_minlines
endif

" Define the default highlighting.
" Only used when an item doesn't have highlighting yet
hi def link tvFormat		tvSpecial
hi def link tvCppString		tvString
hi def link tvCommentL		tvComment
hi def link tvCommentStart	tvComment
hi def link tvLabel		Label
hi def link tvUserLabel		Label
hi def link tvConditional	Conditional
hi def link tvRepeat		Repeat
hi def link tvCharacter		Character
hi def link tvSpecialCharacter	tvSpecial
hi def link tvNumber		Number
hi def link tvOctal		Number
hi def link tvOctalZero		PreProc	 " link this to Error if you want
hi def link tvFloat		Float
hi def link tvOctalError		tvError
hi def link tvParenError		tvError
hi def link tvErrInParen		tvError
hi def link tvErrInBracket	tvError
hi def link tvCommentError	tvError
hi def link tvCommentStartError	tvError
hi def link tvSpaceError		tvError
hi def link tvSpecialError	tvError
hi def link tvCurlyError		tvError
hi def link tvOperator		Operator
hi def link tvStructure		Structure
hi def link tvStorageClass	StorageClass
hi def link tvInclude		Include
hi def link tvPreProc		PreProc
hi def link tvDefine		Macro
hi def link tvIncluded		tvString
hi def link tvError		Error
hi def link tvStatement		Statement
hi def link tvCppInWrapper	tvCppOutWrapper
hi def link tvCppOutWrapper	tvPreCondit
hi def link tvPreConditMatch	tvPreCondit
hi def link tvPreCondit		PreCondit
hi def link tvType		Type
hi def link tvConstant		Constant
hi def link tvCommentString	tvString
hi def link tvComment2String	tvString
hi def link tvCommentSkip	tvComment
hi def link tvString		String
hi def link tvComment		Comment
hi def link tvSpecial		SpecialChar
hi def link tvTodo		Todo
hi def link tvBadContinuation	Error
hi def link tvCppOutSkip		tvCppOutIf2
hi def link tvCppInElse2		tvCppOutIf2
hi def link tvCppOutIf2		tvCppOut2  " Old syntax group for #if 0 body
hi def link tvCppOut2		tvCppOut  " Old syntax group for #if of #if 0
hi def link tvCppOut		Comment

let b:current_syntax = "tv"

unlet s:ft

let &cpo = s:cpo_save
unlet s:cpo_save
" vim: ts=8
