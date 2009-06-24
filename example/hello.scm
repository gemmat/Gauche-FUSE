#!/usr/bin/env gosh
;;;
;;; hello.scm - sample program, hello world filesystem
;;;
;;;   Copyright (c) 2008 Teruaki Gemma <teruakigemma@gmail.com>
;;;
;;;   Redistribution and use in source and binary forms, with or without
;;;   modification, are permitted provided that the following conditions
;;;   are met:
;;;
;;;   1. Redistributions of source code must retain the above copyright
;;;      notice, this list of conditions and the following disclaimer.
;;;
;;;   2. Redistributions in binary form must reproduce the above copyright
;;;      notice, this list of conditions and the following disclaimer in the
;;;      documentation and/or other materials provided with the distribution.
;;;
;;;   3. Neither the name of the authors nor the names of its contributors
;;;      may be used to endorse or promote products derived from this
;;;      software without specific prior written permission.
;;;
;;;   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
;;;   "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
;;;   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
;;;   A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
;;;   OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
;;;   SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
;;;   TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
;;;   PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
;;;   LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
;;;   NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
;;;   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
;;;
;;;  $Id: hello.scm 41 2008-11-24 11:15:57Z teruakigemma $
;;;

(use fuse)

(define (hello-getattr path)
  (define stat (make <sys-stat>))
  (cond 
   ((string=? path "/")
    (sys-stat->mode-set! stat #o0755)
    (sys-stat->file-type-set! stat 'directory)
    (sys-stat->nlink-set! stat 2)
    stat)
   ((string=? path "/hello")
    (sys-stat->mode-set! stat #o0444)
    (sys-stat->file-type-set! stat 'regular)
    (sys-stat->nlink-set! stat 1)
    (sys-stat->size-set! stat (string-size "happy hacking!\n"))
    stat)
   (else (- ENOENT))))

(define (hello-readdir path file-info)
  (if (string=? path "/")
    '("." ".." "hello")
    (- ENOENT)))

(define (hello-open path file-info)
  (if (string=? path "/hello")
    0
    (- ENOENT)))

(define (hello-read path out size offset file-info)
  (if (string=? path "/hello")
    (begin (display "happy hacking!\n" out)
	   size)
    (- ENOENT)))

(define (main args)
  (start-fuse (cadr args)
	      :args args
	      :multithreaded #f
	      :getattr hello-getattr
	      :readdir hello-readdir
	      :open hello-open
	      :read hello-read))