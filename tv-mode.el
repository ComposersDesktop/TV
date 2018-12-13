;; Mode for editing Tabula Vigilans (TV) Programs
;; John Fitch Jan 2014

;; Copyright (C) 2014 by John ffitch

;; Author: John Fitch <jpff@cs.bath.ac.uk>
;; Keywords: TV Tabula, Vigilans, CDP
;; Version: 0.1
(defconst tv-version " 0.1 (tv v2014)")

;;; This file is not part of GNU Emacs.

;;; This program is free software; you can redistribute it and/or modify
;;; it under the terms of the GNU General Public License as published by
;;; the Free Software Foundation; either version 2, or (at your option)
;;; any later version.

;;; This program is distributed in the hope that it will be useful,
;;; but WITHOUT ANY WARRANTY; without even the implied warranty of
;;; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
;;; GNU General Public License for more details.

;;; You should have received a copy of the GNU General Public License
;;; along with GNU Emacs; see the file COPYING.  If not, write to
;;; the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.

;; Revisions: Keywords added 22 Jan 2014

;; Notes: This code has font-lock support
;; Pattern compilation uses make-regexp code

;;; I N S T A L L A T I O N
;;; In order to arrange for autoloading add the following to .emacs
;;; (setq auto-mode-alist (cons '("\\.tv$" . tv-mode) auto-mode-alist))
;;; (autoload 'tv-mode "tv" "Tv major mode." t)
;;;
;;; The running of the mode can be adjusted by various flags best set in
;;; the mode hook.  A typical example is
;;;
;;; (setq tv-mode-hook
;;;       '(lambda ()
;;;          (setq font-lock-maximum-decoration t)
;;;          (turn-on-font-lock)
;;;          (setq tv-mode-do-write-file-hooks t)
;;; ))
;;

(require 'tempo)
(autoload 'make-regexp "make-regexp"    ; We use this sometimes
   "Return a regexp to match a string item in STRINGS.")

;(autoload 'make-regexps "make-regexp"
;   "Return a regexp to REGEXPS.")

;; I recommend you turn these on.

(defvar tv-mode-do-write-file-hooks nil
  "*If not nil, then tv-mode will do timestamps on file saving.
This is done by modifying the local-write-file-hooks.")

;; (see also tempo.el)

;; variables to configure

(defvar tv-mode-never-indent nil
  "*If t, the indentation code for tv-mode is turned off.")

;; hooks (see also tempo.el)

(defvar tv-mode-hook nil
  "*Hook run when tv-mode is started.")

(defvar tv-mode-load-hook nil
  "*Hook run when tv-mode is loaded.")

(defvar tv-mode-timestamp-hook 'tv-default-insert-timestamp
  "*Hook called for timestamp insertion.
Override this for your own timestamp styles.")

(defvar tv-comment-col 48
  "*Column at which to align comments.")

(defvar tv-tab-stops '(12 24 36)
  "*Positions for columns layout.")

;; strings you might want to change

(defvar tv-mode-address-string "TV File"
  "*The default author string of each file.")

(defvar tv-mode-new-buffer-template
  '(
    "//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;\n"
    "// " tv-mode-address-string "\n"
    tv-timestamp-start
    tv-timestamp-end
    "//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;\n"
    "\n\n"
    )
  "*Template for new buffers.
Inserted by tv-insert-new-buffer-strings if 
tv-mode-build-new-buffer is set to t")

(defvar tv-timestamp-start "//****++++\n"
  "*Delimiter for timestamps.
Everything between tv-timestamp-start and tv-timestamp-end will
be deleted and replaced with the output of the function
tv-insert-timestamp if tv-mode-do-write-file-hooks is t")

(defvar tv-timestamp-end "//****----\n"
  "*Delimiter for timestamps.
Everything between tv-timestamp-start and tv-timestamp-end will
be deleted and replaced with the output of the function
tv-insert-timestamp if tv-mode-do-write-file-hooks is t")

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; End of user customisable variables.
;;; Use the functional interface to change the opcodes
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(defvar tv-running-xemacs (string-match "XEmacs\\|Lucid" emacs-version)
  "Set if running on XEmacs")

;; tv basic keymap
(defvar tv-mode-map (make-sparse-keymap) "Keymap for tv-mode")

;; special mode keys
;; (define-key tv-mode-map ":"        'tv-colon)
;; (define-key tv-mode-map ";"        'tv-comment)
(define-key tv-mode-map "\C-c;"    'comment-region)
(define-key tv-mode-map "\C-c:"    'uncomment-region)
;;(define-key tv-mode-map "\C-c\C-a" 'tv-ins-add-keyword)
(define-key tv-mode-map "\C-c\C-c" 'comment-region)
;;(define-key tv-mode-map "\C-c\C-d" 'tv-ins-kill-keyword)
(define-key tv-mode-map "\M-\C-t"
  'tv-insert-timestamp-delimiter-at-point)
(define-key tv-mode-map [(control c) (control ?2)] 'tv-e-power)

;; indentation keys
(define-key tv-mode-map "\t" 'tv-mode-indent-command)
(define-key tv-mode-map "\C-j" 'newline-and-indent)

(mapc
 (function (lambda (l) (define-key tv-mode-map (car l) (nth 1 l))))
 '(("\M-\C-f" tempo-forward-mark)
   ("\M-\C-b" tempo-backward-mark)
   ("\M-\t"   tempo-complete-tag)))

(defvar tv-mode-syntax-table nil "Syntax table")

(if tv-mode-syntax-table
    ()
  (setq tv-mode-syntax-table
        (make-syntax-table text-mode-syntax-table))
;;
;;  (modify-syntax-entry ?'  "w   " tv-mode-syntax-table)
)


(defvar tv-equals nil
  "*Assignment operator")
(setq tv-equals "=")


(defvar tv-rules nil
  "list of opcodes for indenting")
(defvar tv-rules-raw nil "*List of rules")

(setq tv-rules-raw
      '(
        "add_dec"
        "alllocked"
        "anylocked"
        "compare"
        "control_out"
        "copy"
        "copy_table"
        "embed"
        "exp"
        "fail"
        "fold"
        "generate"
        "interp_table"
        "lim"
        "lin"
        "lintrans"
        "lock"
        "log"
        "max"
        "mean"
        "message"
        "messageq"
        "messag1"
        "min"
        "midichord"
        "midiecho"
        "midiin"
        "midiout"
        "midiset"
        "mouse"
        "mult"
        "mult_table"
        "offset_table"
        "perm"
        "pitchbend"
        "pop"
        "print"
        "probe"
        "probi"
        "schedule"
        "scale_table"
        "seg"
        "shift"
        "sort"
        "store"
        "stor4"
        "stor6"
        "stori"
        "storf"
        "subst"
        "sum"
        "sum_table"
        "swap"
        "time"
        "trigger"
        "unlock"
        "wait"
        "xad"
        "xar"
        ))

;;; Rules
(quote
 (setq max-specpdl-size 1500)
)
(quote
 (concat "\\<\\(" (make-regexp tv-rules-raw) "\\)\\>")
)

(setq tv-rules
      "\\<\\(a\\(dd_dec\\|lllocked\\|nylocked\\)\\|co\\(mpare\\|ntrol_out\\|py\\(\\|_table\\)\\)\\|e\\(mbed\\|xp\\)\\|f\\(ail\\|old\\)\\|generate\\|interp_table\\|l\\(i\\([mn]\\|ntrans\\)\\|o\\(ck\\|g\\)\\)\\|m\\(ax\\|e\\(an\\|ssag\\([1e]\\|eq\\)\\)\\|i\\(di\\(chord\\|echo\\|in\\|out\\|set\\)\\|n\\)\\|ouse\\|ult\\(\\|_table\\)\\)\\|offset_table\\|p\\(erm\\|itchbend\\|op\\|r\\(int\\|ob[ei]\\)\\)\\|s\\(c\\(ale_table\\|hedule\\)\\|eg\\|hift\\|ort\\|tor[46efi]\\|u\\(bst\\|m\\(\\|_table\\)\\)\\|wap\\)\\|t\\(ime\\|rigger\\)\\|unlock\\|wait\\|xa[dr]\\)\\>"
)


;;; keywords
(defvar tv-keywords nil
  "list of void opcodes for indenting.")
(defvar tv-keywords-raw nil
  "*List of opcodes with no result.")
(setq tv-keywords-raw
      '(
        "break"
        "call"
        "case"
        "cls"
        "continue"
        "store_digits"
        "default"
        "else"
        "end"
        "fill_table"
        "for"
        "if"
        "local"
        "loop"
        "return"
        "oscsend"
        "storefile"
        "storstr"
        "switchon"
        "system"
        "table"
        "try"
        "while"
))

(quote
 (concat "\\b\\(" (make-regexp tv-keywords-raw ) "\\)\\b")
)

(setq tv-keywords "\\b\\(break\\|c\\(a\\(ll\\|se\\)\\|ls\\|ontinue\\)\\|default\\|e\\(lse\\|nd\\)\\|f\\(ill_table\\|or\\)\\|if\\|lo\\(cal\\|op\\)\\|oscsend\\|return\\|s\\(tor\\(e\\(_digits\\|file\\)\\|str\\)\\|witchon\\|ystem\\)\\|t\\(able\\|ry\\)\\|while\\)\\b"
)

;;; Iunctions
(defvar tv-functions nil
  "list of functions for indenting")
(defvar tv-functions-raw nil "list of I/O opcodes.")
(setq tv-functions-raw '(
                         "abs"
                         "argc"
                         "arg"
                         "dimensions"
                         "dimsize"
                         "int"
                         "gamma"
                         "gauss"
                         "power"
                         "rand"
                         "random"
                         "round"
                         "sine"
                         "sqrt"
                         "cosine"
                         "tangent"
                         "arcsine"
                         "arccosine"
                         "arctangent"
                         "natlog"
                         "log10"
                         ))
(quote
   (concat "\\b\\(" (make-regexp tv-functions-raw) "\\)\\b")
)
(setq tv-functions
"\\b\\(a\\(bs\\|r\\(c\\(cosine\\|sine\\|tangent\\)\\|gc?\\)\\)\\|cosine\\|dim\\(ensions\\|size\\)\\|ga\\(mma\\|uss\\)\\|int\\|log10\\|natlog\\|power\\|r\\(and\\(\\|om\\)\\|ound\\)\\|s\\(ine\\|qrt\\)\\|tangent\\)\\b"
)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; Functions to maintain opcodes
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(defvar tv-rules-change nil "Set when list changes")
(defvar tv-keywords-change nil "Set when list changes")
(defvar tv-functions-change nil "Set when list changes")

(defun delete-from-list (x l)
  (cond ((null l) nil)
        ((equal x (car l)) (delete-from-list x (cdr l)))
        (t (cons (car l) (delete-from-list x (cdr l))))))

(defun make-tv-patterns ()
  "If the lists of opcodes have changed remake the patterns"
  (if tv-rules-change
      (setq tv-rules
            (concat "\\<\\(" (make-regexp tv-rules-raw) "\\)\\>")))
  (if tv-keywords-change
      (setq tv-keywords
            (concat "\\<\\(" (make-regexp tv-keywords-raw) "\\)\\>")))
  (if tv-functions-change
      (setq tv-functions
            (concat "\\<\\(" (make-regexp tv-functions-raw) "\\)\\>")))
  (setq tv-rules-change nil)
  (setq tv-keywords-change nil)
  (setq tv-functions-change nil)
)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; Indentation
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(defvar tv-search-limit 2000 "limit on how far back we search")

(defvar tv-mode-print-indent-info nil
  "If t, indent will print out information as a message.")

(defun tv-mode-indent-command ()
  "Command for indenting text.
Just calls tv-mode-indent."
  (interactive)
  (tv-mode-indent))

;; some of the ideas are borrowed from cc-mode.el.

(defun tv-mode-indent ()
  "indentation workhorse function."
  (if tv-mode-never-indent ()
    (let ((m (point-marker))
          (ateol (eolp))
          (pp nil))
      ;; unindent the line
      (end-of-line)
      (setq pp (point))
      (beginning-of-line)
      (save-restriction
        (narrow-to-region (point) pp)
        (if (looking-at "^;") ()        ; Comment at start of line is OK
          (delete-region (point) (progn (back-to-indentation) (point)))
          (if (looking-at ";;;") ()     ; Three comments goes to start
            (if (looking-at ";;")
                (indent-to 8)           ;Two to short state
              (if (looking-at "\\sw+:")
                  (progn
                    (search-forward ":")
                    (if (looking-at "[ \t]*;")
                        (progn
                          (delete-horizontal-space)
                          (indent-to comment-column))
                      (if (looking-at "[ \t]*$")
                          (delete-horizontal-space)
                        (forward-char 1)
                        (delete-horizontal-space)
                        (insert "\n")
                        (tv-mode-indent))))
                (if (looking-at "#") ()
            ;;; General case
                  (if (not (looking-at "^[ \t]*$")) ;If empty line do nothing
                      (progn
                        (delete-horizontal-space) ;Remove initial space
                        (if (looking-at "\\bif\\b") ;A Conditional
                            (progn
                              (goto-char (match-end 0)) ;Skip the "if"
                              (delete-horizontal-space)
                              (insert " ")
                              (insert "%") (zap-to-char -1 ?%)
                                        ;Need to skip boolean!!
                              (re-search-forward "[ \t]+")
                              (insert "%") (zap-to-char -1 ?%)
                              (delete-horizontal-space)
                              (tab-to-tab-stop)))
                        (insert "%") (zap-to-char -1 ?%)
                        (if             ;If an opcode with no answer
                            (looking-at tv-keywords)
                            (tab-to-tab-stop)   ;need another tab
                            (insert "%") (zap-to-char -1 ?%)
                            (insert "  ")  ;Space at start of line
                            (re-search-forward "\\b\\sw+\\b") ;or skip answer(s)
                            (if (looking-at ",")  ; A second answer; minimal checks
                                (skip-chars-forward "a-zA-Z0-9,"))
                            (insert "%") (zap-to-char -1 ?%)
                            (delete-horizontal-space) ;and retab
                            (tab-to-tab-stop)
                            (insert "%") (zap-to-char -1 ?%)
                            (if (not (looking-at tv-equals))
                                (message "Unknown opcode")))
                                        ;We now need to deal with argument
                          (if (re-search-forward
                               "\\(\\b[a-zA-Z0-9]+\\b\\|=\\)[ \t]*" (point-max) t)
                              (progn
                                (insert "%") (zap-to-char -1 ?%)
                                (if (looking-at "[a-zA-Z0-9(]") ;If an argument
                                    (delete-horizontal-space))
                                (tab-to-tab-stop)))
                          (insert "%") (zap-to-char -1 ?%)
                          (while (and (not (looking-at "[ \t]*;"))
                                      (< (point) (point-max)))
                            (insert "%") (zap-to-char -1 ?%)
                            (if (looking-at "[ \t]")
                                (delete-char 1)
                              (if (looking-at "\"")
                                  (progn (forward-char 1)
                                         (search-forward "\""))
                                (if (not (looking-at "[,?:]"))
                                    (forward-char 1)
                                  (forward-char 1)
                                  (insert " ")))))
                          (if (looking-at "[ \t]*;")        ; if there is a comment left
                              (progn
                                (delete-horizontal-space)
                                (search-forward ";")
                                (backward-char 1)
                                (indent-to
                                 (max (1+ (current-column)) comment-column)))))))))))
        ;; adjust point to where it was before, or at start of indentation
        (if ateol
            (end-of-line)
          (goto-char (marker-position m)))
        )
      (if (< (current-column) (current-indentation))
          (back-to-indentation)))))

(defun tv-mode-comment ()
  (save-excursion
    (beginning-of-line)
    (if (looking-at "//")
        0
      (skip-chars-backward " \t")
      (max (if (bolp) 0 (1+ (current-column)))
           comment-column))))

(defun tv-indent-line ()
  "Indents current Tv line based on its contents."
  (interactive)
  (tv-mode-indent)
;  (let ((csi (calculate-tv-indent)))
;    (save-excursion
;      (beginning-of-line)
;      (if (not (= csi (tv-current-line-indentation)))
;         (indent-to csi)
;       (beginning-of-line)
;       (if (and (not (looking-at "^;"))
;                (tv-find-comment-start-skip))
;           (tv-indent-comment)))))
  )

(defun tv-indent-comment ()
  "Align or create comment on current line.
Existing comments of all types are recognized and aligned.
If the line has no comment, a side-by-side comment is inserted and aligned
if the value of  comment-start  is not nil.
Otherwise, a separate-line comment is inserted, on this line
or on a new line inserted before this line if this line is not blank."
  (interactive)
  (beginning-of-line)
  ;; Recognize existing comments
  (cond ((looking-at ";")
         (tv-indent-line))
        ; otherwise goto end of line or sth else?
        ;; No existing comment.
        ;; If side-by-side comments are defined, insert one,
        ;; unless line is now blank.
        ((and comment-start (not (looking-at "^[ \t]*$")))
         (end-of-line)
         (delete-horizontal-space)
         (indent-to (tv-mode-comment))
         (insert comment-start))
        ;; Else insert separate-line comment, making a new line if nec.
        (t
         (if (looking-at "^[ \t]*$")
             (delete-horizontal-space)
;;         (beginning-of-line)
;;         (insert "\n")
;;         (forward-char -1)
           )
         (insert ";")
         (insert-char '\; (- (calculate-tv-indent) (current-column))))))

(defun tv-find-comment-start-skip ()
  "Move to past `comment-start-skip' found on current line.
Return t if `comment-start-skip' found, nil if not."
;;; In order to move point only if comment-start-skip is found,
;;; this one uses a lot of save-excursions.  Note that re-search-forward
;;; moves point even if comment-start-skip is inside a string-constant.
;;; Some code expects certain values for match-beginning and end
  (interactive)
  (if (save-excursion
        (re-search-forward comment-start-skip
                           (save-excursion (end-of-line) (point)) t))
      (let ((save-match-beginning (match-beginning 0))
            (save-match-end (match-end 0)))
        (goto-char save-match-beginning)
        (re-search-forward comment-start-skip
                           (save-excursion (end-of-line) (point)) t)
        (goto-char (match-end 0))
        t)
    nil))

(defun tv-current-line-indentation ()
  "Indentation of current line
This is the column position of the first non-whitespace character"
  (save-excursion
    (beginning-of-line)
    (cond ((looking-at "^;")
           (goto-char (match-end 0))
           (skip-chars-forward ";")))
    ;; Move past whitespace.
    (skip-chars-forward " \t")
    (current-column)))

(defun calculate-tv-indent ()
  "Calculates the Tv indent column based on previous lines."
  (let (icol)
    (save-excursion
      (if (= (point) (point-min))
          (setq icol 8)
        (save-excursion
          (beginning-of-line)
          (cond ((looking-at "^;")
                 (setq icol 0))
          ;; Move past whitespace.
                (t (skip-chars-forward " \t")
                   (setq icol (current-column))))))
      (save-excursion
        (beginning-of-line)
        icol))))

;;;; Electric characters


;; timestamps

(defun tv-update-timestamp ()
  "Basic function for updating timestamps. It finds the timestamp in
the buffer by looking for tv-timestamp-start, deletes all text
up to tv-timestamp-end, and runs tv-mode-timestamp-hook
which will presumably insert an appropriate timestamp in the buffer."
  (save-excursion
    (save-restriction
      (widen)
      (goto-char (point-max))
      (if (not (search-backward tv-timestamp-start nil t))
          (message "timestamp delimiter start was not found")
        (let ((ts-start (+ (point) (length tv-timestamp-start)))
              (ts-end (if (search-forward tv-timestamp-end nil t)
                          (- (point) (length tv-timestamp-end))
                        nil)))
          (if (not ts-end)
              (message "timestamp delimiter end was not found. Type C-c C-t to insert one.")
            (delete-region ts-start ts-end)
            (goto-char ts-start)
            (run-hooks 'tv-mode-timestamp-hook)))))
    nil))

(defun tv-default-insert-timestamp ()
  "Default timestamp insertion function"
  (insert ";**** Last modified: "
          (current-time-string)
          "\n"))

(defun tv-insert-timestamp-delimiter-at-point ()
  "Simple function that inserts timestamp delimiters at point.
Useful for adding timestamps to existing buffers."
  (interactive)
  (insert tv-timestamp-start)
  (insert tv-timestamp-end))

;; tv-insert-new-buffer-strings

(tempo-define-template "tv-skeleton" tv-mode-new-buffer-template
                       nil
                       "Insert a skeleton for a TV document")

(defun tv-insert-new-buffer-strings ()
  "Insert tv-mode-new-buffer-strings."
  (tempo-template-tv-skeleton)
)

(defvar tv-code-level-empty-comment-pattern nil)
(defvar tv-flush-left-empty-comment-pattern nil)
(defvar tv-inline-empty-comment-pattern nil)

;;; Now an attempt at font-lock support

(defconst tv-font-lock-keywords-1 nil
  "Subdued level highlighting for Tv mode.")

(defconst tv-font-lock-keywords-2 nil
  "Medium level highlighting for Tv mode.")

(defconst tv-font-lock-keywords-3 nil
  "Gaudy level highlighting for Tv mode.")

(defconst tv-font-lock-keywords nil
  "Default expressions to highlight in Tv mode.")

(let (
      (tv-globals "\\b\\(sr\\|k\\(r\\|smps\\)\\|nchnls\\)\\b")
      (tv-labels "^[ \t]*\\sw+[ \t]*:")
      (tv-funcs
       "\\b\\([ip]\\|a\\(bs\\|mpdb\\(\\|fs\\)\\)\\|birnd\\|c\\(ent\\|os\\(\\|h\\|inv\\)\\|ps\\(oct\\|pch\\)\\)\\|d\\(b\\(\\|amp\\(\\|fs\\)\\)\\|userrnd\\)\\|exp\\|f\\(rac\\|t\\(chnls\\|l\\(en\\|ptim\\)\\)\\)\\|int\\|log\\|oct\\(ave\\|cps\\|pch\\)\\|pchoct\\|rnd\\|s\\(emitone\\|in\\(\\|h\\|inv\\)\\|qrt\\)\\|tan\\(\\|h\\|inv\\)\\|urd\\)\\b"
       ))
  (setq tv-font-lock-keywords-1
        (list
         ;; Comments
         ;;
         ;; Fontify syntactically (assuming strings cannot be quoted or span lines).
         '(";.*$" . font-lock-tv-comment)
         '("\"[^\"\n]*\""  . font-lock-tv-string)
         ;;
         ;; Program, subroutine and function declarations, plus calls.
         (cons tv-equals 'font-lock-tv-equals)
         (cons tv-keywords 'font-lock-tv-keywords)))

  (setq tv-font-lock-keywords-2
        (append
         tv-font-lock-keywords-1
         (list
          ;;
          ;; Fontify all type specifiers (must be first; see below).
          (cons tv-keywords 'font-lock-tv-opcode)
          ;;
          ;; Labels
          '("^[ \t]*\\(\\b[a-zA-Z0-9]+\\b\\):" (1 font-lock-tv-label))
          ;;
          )))

  (setq tv-font-lock-keywords-3
        (append
         ;;
         ;; The list `tv-font-lock-keywords-2'.
         tv-font-lock-keywords-2
         ;;
         (list
          ;; Things extra to `tv-font-lock-keywords-3' (must be done first).
          (cons tv-funcs 'font-lock-tv-funcs)
          ;;
          )))
  )

;;
;; tv-mode

(defun tv-mode ()
  "Mode for editing TV Programs

\\{tv-mode-map}
Major mode for editing TV Programs.
Written by John Fitch (jpff@codemist.co.uk)"
  (interactive)
  (kill-all-local-variables)

  (use-local-map tv-mode-map)
  (set-syntax-table tv-mode-syntax-table)

  (setq mode-name "TV")
  (setq major-mode 'tv-mode)

  (make-local-variable 'comment-start)
  (make-local-variable 'comment-end)
  (make-local-variable 'comment-column)
  (make-local-variable 'indent-line-function)

  (setq comment-start "; "
        comment-end ""
        comment-column tv-comment-col
        indent-line-function 'tv-indent-line)
  (make-local-variable 'comment-indent-function)
  (setq comment-indent-function 'tv-mode-comment)
  (make-local-variable 'comment-start-skip)
  (setq comment-start-skip "//+[ \t]*")
  (setq tv-inline-empty-comment-pattern "^.+//+ *$")
  (setq tv-code-level-empty-comment-pattern "^[\t ]+// *$")
  (setq tv-flush-left-empty-comment-pattern "^/// *$")
  (make-local-variable 'indent-tabs-mode)
  (setq indent-tabs-mode nil)
  (make-local-variable 'tab-stop-list)
  (setq tab-stop-list (append tv-tab-stops
                              (cons tv-comment-col nil)))

;; Font lock
  (make-local-variable 'font-lock-defaults)
;;  (setq tv-font-lock-keywords tv-font-lock-keywords-2)
  (setq tv-font-lock-keywords tv-font-lock-keywords-3)
  (setq font-lock-defaults '((tv-font-lock-keywords
                              tv-font-lock-keywords-1
                              tv-font-lock-keywords-2
                              tv-font-lock-keywords-3)
                             nil nil nil beginning-of-defun
                             (font-lock-comment-start-regexp . ";")
                             (font-lock-mark-block-function mark-defun)))
  (let ((colour-faces
         (if tv-running-xemacs 
             '(;; Format is:
               ;; face                              "colour"        "background"
               ;; adapted faces to XEmacs conventions 
               ;; change colours to suite your needs
               (font-lock-tv-string                 "green4"            nil)
               (font-lock-tv-comment                "red"               nil)
               (font-lock-tv-rules                  "orange"            nil)
               (font-lock-tv-equals                 "blue"              nil) 
               (font-lock-tv-keywords               "grey50"            nil) 
               (font-lock-tv-functions              "blue"              nil) 
               )
           '(;; Format is:
             ;; face                              "colour"        bold
             (font-lock-tv-string                 "magenta"       nil)
             (font-lock-tv-comment                "grey50"        nil)
             (font-lock-tv-globs                  "purple"        nil)
             (font-lock-tv-rules                  "firebrick"     nil)
             ;;(font-lock-tv-inout                  "Navy"          nil)
             ;;(font-lock-tv-opcode-adi             "MediumAquamarine"      nil)
             ;;(font-lock-tv-irate                  "ForestGreen"   nil)
             ;;(font-lock-tv-krate                  "RoyalBlue"     nil)
             (font-lock-tv-functions              "DodgerBlue"    nil)
             ;;(font-lock-tv-frate                  "MediumBlue"    nil)
             (font-lock-tv-keywords               "DarkGoldenrod" nil)
             ;;(font-lock-tv-funcs                  "Sienna"        nil)
             ;;(font-lock-tv-inst                   "red"           t)
             (font-lock-tv-equals                 "red"           t)
             ;;(font-lock-tv-label                  "red"           t)
             ;;(font-lock-tv-decls                  "RoyalBlue"     t)
             ;;(font-lock-tv-flow                   "blue"          t)
           )))
        face colour colour-face colour-bdbg)
    (while colour-faces
      (setq face (nth 0 (car colour-faces))
            colour (nth 1 (car colour-faces))
            colour-face (if tv-running-xemacs face
                          (intern (concat (symbol-name face) "-colour")))
            colour-bdbg (nth 2 (car colour-faces))
            colour-faces (cdr colour-faces))
      (make-face colour-face)
      (if colour-bdbg (if tv-running-xemacs (set-face-background colour-face colour-bdbg)
                        (make-face-bold colour-face nil t)))
      (set-face-foreground colour-face colour)
      (set face colour-face)))
  (font-lock-mode 1)
  ;; added menus <Steve Kersten>
  (tv-install-menu "TV")
  (run-hooks 'tv-mode-hook)
  (if tv-mode-do-write-file-hooks
      (add-hook 'local-write-file-hooks 'tv-update-timestamp))

  (add-hook 'local-write-file-hooks 'tv-check-rates)

  (if (and tv-mode-build-new-buffer (zerop (buffer-size)))
        (tv-insert-new-buffer-strings)))



(defvar tv-instrument-template1 "\n\n;;; I N S T R U M E N T    "
  "Text at start of new instument")

(defvar tv-instrument-template2
  "\n;;; Write description here\n;;; Keywords:\n"
  "Text at start of new instument")

;;; Deal with comments
(defun tv-line-matches (pattern &optional withcomment)
  (save-excursion
    (beginning-of-line)
    (looking-at pattern)))

(defun tv-pop-comment-level ()
  ;; Delete an empty comment ending current line.  Then set up for a new one,
  ;; on the current line if it was all comment, otherwise above it
  (end-of-line)
  (delete-horizontal-space)
  (while (= (preceding-char) ?;
            )
    (delete-char -1))
  (delete-horizontal-space)
  (if (bolp)
      nil
    (beginning-of-line)
    (open-line 1)))

(defun tv-comment ()
  "Convert an empty comment to a `larger' kind, or start a new one.
These are the known comment classes:

   1 -- comment to the right of the code (at the comment-column)
   2 -- comment on its own line, indented like code
   3 -- comment on its own line, beginning at the left-most column.

Suggested usage:  while writing your code, trigger tv-comment
repeatedly until you are satisfied with the kind of comment."
  (interactive)
  (cond

   ;; Blank line?  Then start comment at code indent level.
   ((tv-line-matches "^[ \t]*$")
    (delete-horizontal-space)
    (tab-to-tab-stop)
    (insert comment-start))

   ;; Nonblank line with no comment chars in it?
   ;; Then start a comment at the current comment column
   ((tv-line-matches "^[^;\n]+$")
    (indent-for-comment))

   ;; Flush-left comment present?  Just insert character.
   ((tv-line-matches tv-flush-left-empty-comment-pattern)
    (insert ";"))

   ;; Empty code-level comment already present?
   ;; Then start flush-left comment, on line above if this one is nonempty.
   ((tv-line-matches tv-code-level-empty-comment-pattern)
    (tv-pop-comment-level)
    (insert ";;;"))

   ;; Empty comment ends line?
   ;; Then make code-level comment, on line above if this one is nonempty.
   ((tv-line-matches tv-inline-empty-comment-pattern)
    (tv-pop-comment-level)
    (tab-to-tab-stop)
    (insert ";; "))

   ;; If all else fails, insert character
   (t
    (insert ";")))
  (end-of-line))

(defun uncomment-region ()
  "Call `\\[comment-region]' with an argument"
  (interactive)
  (uncomment-region (point) (mark)))

(defun indent-buffer ()
  "*Relayout entire buffer"
  (interactive)
  (indent-region (point-min) (point-max) 0))


;;;
;;; tv-add-ons by Steve Kersten, contact steve-k@gmx.net
;;;


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;
;;; MENUS
;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(defvar tv-mode-menu nil "XEmacs menu")
(setq tv-mode-menu
      '(
        "TV"
        ["Indent Region" indent-region t]
        ["Indent Buffer" indent-buffer t]
        ["Comment Region" comment-region t]
        ["Uncomment Region" uncomment-region t]
        ["Power-of-2 Region" e-power t]
        "-----"
        ["Report Bug" tv-submit-bug-report t]))

(autoload 'csdoc-document-opcode "tv-doc" "Tv Documentation browsing." t)

(defun tv-install-menu (menu-name)
  (if (and tv-running-xemacs (featurep 'menubar)
           current-menubar (not (assoc menu-name current-menubar)))
      (progn
        (set-buffer-menubar (copy-sequence current-menubar))
        (add-submenu nil
                     (cons menu-name (cdr tv-mode-menu))
                     "Tv/Orc"))))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; EDITING FUNCTIONS
;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;; computes power of 2 next to x, returns nil for x=0

(defun e-power-of-2 (x)
  (let (
        (min 2)           ;; Minimum of computation -- at least 2
        (max (expt 2 24)) ;; Maximum of computation -- 2^24 for tv-GEN sizes
        (round-log 0)
        (y 0)
        (z 0))
    
    (if (= x 0) nil
      (progn               
        (if (<= x min) 
            (setq x min)
          (progn 
            (if (>= x max) 
                (setq x max)
              (progn
                (setq round-log (truncate (log x 2)))
                (setq y (expt 2 round-log))
                (setq z (expt 2 (1+ round-log)))
                (if (>= (abs (/ (- x y) (- x z))) 1) 
                    z 
                  y)))))))))

(defun tv-e-power (start end)
  (interactive "r")
  (let ((number 0))
    (when end
      (save-restriction
        (narrow-to-region start end)
        (setq number (string-to-number (buffer-string))
            number (e-power-of-2 number))
        (when number 
          (delete-region start end)
          (insert (format "%d" number)))))))


;; Menus
(if (not tv-running-xemacs)
    (progn
      (define-key tv-mode-map [menu-bar tv]
        (cons "TV" (make-sparse-keymap "TV")))
      (put 'uncomment-region 'menu-enable 'mark-active)
      (put 'tv-e-power 'menu-enable 'mark-active)
      (define-key tv-mode-map [menu-bar tv Submit-Report]
        '("Report Bug" . tv-submit-bug-report))
      (define-key tv-mode-map [menu-bar tv Indent-Region]
        '("Indent Region" . indent-region))
      (define-key tv-mode-map [menu-bar tv Indent-buffer]
        '("Indent Buffer" . indent-buffer))
      (define-key tv-mode-map [menu-bar tv UnComment-Region]
        '("UnComment Region" . uncomment-region))
      (define-key tv-mode-map [menu-bar tv Comment-Region]
        '("Comment Region" . comment-region))
      (define-key tv-mode-map [menu-bar tv Power-Region]
        '("Power-of-2 Region" . tv-e-power))
))

;; Debug support

(defun tv-submit-bug-report ()
  "Submit via mail a bug report on tv.el."
  (interactive)
  (require 'reporter)
  (let ((reporter-prompt-for-summary-p t))
    (reporter-submit-bug-report "jpff@maths.bath.ac.uk" (concat "tv" tv-version)
     '(tv-mode-do-write-file-hooks
       tv-mode-build-new-buffer
       tv-mode-never-indent
       tv-mode-hook
       tv-mode-load-hook
       tv-mode-timestamp-hook
       tv-comment-col
       tv-tab-stops
       tv-mode-address-string
       tv-mode-new-buffer-template
       tv-timestamp-start
       tv-timestamp-end
       tv-keywords-msg
       tv-decls
       tv-equals
       tv-keywords
       tv-keywords-raw
       tv-keywords
       tv-keywords-raw
       tv-rules
       tv-rules-raw
       tv-functions
       tv-functions-raw
       tv-search-limit
       tv-mode-print-indent-info
       tv-last-key
       )                                ; List of variables
     nil nil
     (concat "Hi John.,

I want to report a bug in tv mode

"))))

;;;;;;;; Templates for keywords

(defun tv-abbrev ()
  "Expand Tv opcode"
  (interactive)
  (let (abbrev expansions (oplist all-ops))
    (if (bobp)
        (error "No possible abbreviation preceding point"))
    ;; Return abbrev at point
    (save-excursion
      ;; Record the end of the abbreviation.
      (setq last-abbrev-location (point))
      (save-match-data
        (if (search-backward-regexp "[ \t\n]" 0 t)
            (forward-char 1)
          (error "No possible abbreviation preceding point"))
        (if (looking-at "\\(\\sw\\|\\s_\\)+")
      ;; Now find the beginning of that one.
            (setq abbrev 
                  (concat "^"
                          (buffer-substring-no-properties
                           (point) last-abbrev-location)))
          (error "No possible abbreviation preceding point")))
      (delete-region last-abbrev-location (point)) ;; Remove string
      (while oplist                     ;; Look for all possible expansions
        (if (string-match abbrev (caar oplist))
            (let (new)
              (setq new (list (cadr (car oplist)) (car (car oplist)) (car (cddr (car oplist)))))
              (setq expansions (cons new expansions))
              (setq oplist (cdr oplist)))
          (if expansions (setq oplist nil)
            (setq oplist (cdr oplist)))))
      (if expansions
          (let ((start (point)))
            (setq expansions (reverse expansions))
            (tv-insert-abbrev (car expansions))
            (while (and expansions (not (y-or-n-p "Accept? ")))
              (delete-region start (point))
              (setq expansions (cdr expansions))
              (if expansions (tv-insert-abbrev (car expansions))))
            (insert abbrev)
            (save-excursion
              (search-backward "^")
              (delete-char 1)))
        (insert abbrev)
        (save-excursion
          (search-backward "^")
          (delete-char 1))) )))

(defun tv-insert-abbrev (expand)
  (let ((n 0))
    (setq n (tv-insertvars n (car expand)))
    (insert " ")
    (tv-insertop (cadr expand))
    (insert " ")
    (tv-insertvars n (car (cddr expand)))))

(define-key tv-mode-map "\e/"        'tv-abbrev)

(provide 'tv-mode)
(run-hooks 'tv-mode-load-hook)

;;; tv-mode.el ends here
