#!/usr/bin/env gosh
;;;
;;; fusexmp.scm - sample program, fusexmp filesystem
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
;;;  $Id: fusexmp.scm 41 2008-11-24 11:15:57Z teruakigemma $
;;;

(use fuse)

(define (fusexmp-readdir path file-info)
  (sys-readdir path))

(define (fusexmp-mknod path mode rdev)
  (define st (make <sys-stat>))
  (sys-stat->mode-set! st mode)
  (case (slot-ref st 'type)
    ('regular
     (close-output-port (open-output-file path)))
    ('fifo
     (sys-mkfifo path mode))
    (else
     ;;please use the "mknod" external command.
     (error <system-error> :errno ENOENT))))

(define (fusexmp-open path file-info)
  (close-input-port (open-input-file path)))

(define (fusexmp-read path outp size offset file-info)
  (call-with-input-file path (lambda (in)
			       (port-seek in offset)
			       (copy-port in outp)))
  size)

(define (fusexmp-write path inp size offset file-info)
  (call-with-output-file path (lambda (out)
				(port-seek inp offset)
				(copy-port inp out)))
  size)

(define (main args)
  (start-fuse (cadr args)
	      :args args
	      :multithreaded #f
	      :getattr sys-lstat
	      :access sys-access
	      :readlink sys-readlink
	      :mkdir sys-mkdir
	      :symlink sys-symlink
	      :unlink sys-unlink
	      :rmdir sys-rmdir
	      :rename sys-rename
	      :link sys-link
	      :chmod sys-chmod
	      :chown sys-chown
	      :truncate sys-truncate
	      :utimens sys-utime
	      :statfs sys-statvfs
	      :readdir fusexmp-readdir
	      :mknod fusexmp-mknod
	      :open fusexmp-open
	      :read fusexmp-read
	      :write fusexmp-write))

