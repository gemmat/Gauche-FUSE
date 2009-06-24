;;;
;;; fuse.scm - Gauche FUSE binding
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
;;;  $Id: fuse.scm 29 2008-11-23 18:50:11Z teruakigemma $
;;;

(define-module fuse
  (use gauche.threads)
  (export test-fuse ;; dummy
	  start-fuse
	  sys-stat->file-type-set!
	  sys-stat->perm-set!
	  sys-stat->mode-set!
	  sys-stat->ino-set!
	  sys-stat->dev-set!
	  sys-stat->rdev-set!
	  sys-stat->nlink-set!
	  sys-stat->size-set!
	  sys-stat->uid-set!
	  sys-stat->gid-set!
	  sys-stat->atime-set!
	  sys-stat->mtime-set!
	  sys-stat->ctime-set!
	  sys-statvfs
	  <sys-statvfs>
	  fuse-file-info->flags
	  fuse-file-info->flags-set!
	  fuse-file-info->writepage
	  fuse-file-info->writepage-set!
	  fuse-file-info->direct_io
	  fuse-file-info->direct_io-set!
	  fuse-file-info->keep_cache
	  fuse-file-info->keep_cache-set!
	  fuse-file-info->flush
	  fuse-file-info->flush-set!
	  fuse-file-info->fh
	  fuse-file-info->fh-set!
	  fuse-file-info->lock_owner
	  fuse-file-info->lock_owner-set!
          )
  )
(select-module fuse)

;; Loads extension
(dynamic-load "fuse")

;;
;; Put your Scheme definitions here
;;

(define (call-getattr path)
  (guard (e ((condition-has-type? e <system-error>) (- (condition-ref e 'errno)))
	    ((condition-has-type? e <message-condition>) (print ":getattr " (condition-ref e 'message)) (- ERANGE))
	    (else (- ERANGE)))
	 (your-getattr path)))

(define (call-readlink path)
  (guard (e ((condition-has-type? e <system-error>) (- (condition-ref e 'errno)))
	    ((condition-has-type? e <message-condition>) (print ":readlink " (condition-ref e 'message)) (- ERANGE))
	    (else (- ERANGE)))
	 (your-readlink path)))

(define (call-mknod path mode rdev)
  (guard (e ((condition-has-type? e <system-error>) (- (condition-ref e 'errno)))
	    ((condition-has-type? e <message-condition>) (print ":mknod " (condition-ref e 'message)) (- ERANGE))
	    (else (- ERANGE)))
	 (your-mknod path mode rdev)))

(define (call-mkdir path mode)
  (guard (e ((condition-has-type? e <system-error>) (- (condition-ref e 'errno)))
	    ((condition-has-type? e <message-condition>) (print ":mkdir " (condition-ref e 'message)) (- ERANGE))
	    (else (- ERANGE)))
	 (your-mkdir path mode)))

(define (call-unlink path)
  (guard (e ((condition-has-type? e <system-error>) (- (condition-ref e 'errno)))
	    ((condition-has-type? e <message-condition>) (print ":unlink " (condition-ref e 'message)) (- ERANGE))
	    (else (- ERANGE)))
	 (your-unlink path)))

(define (call-rmdir path)
  (guard (e ((condition-has-type? e <system-error>) (- (condition-ref e 'errno)))
	    ((condition-has-type? e <message-condition>) (print ":rmdir " (condition-ref e 'message)) (- ERANGE))
	    (else (- ERANGE)))
	 (your-rmdir path)))

(define (call-symlink from to)
  (guard (e ((condition-has-type? e <system-error>) (- (condition-ref e 'errno)))
	    ((condition-has-type? e <message-condition>) (print ":symlink " (condition-ref e 'message)) (- ERANGE))
	    (else (- ERANGE)))
	 (your-symlink from to)))

(define (call-rename from to)
  (guard (e ((condition-has-type? e <system-error>) (- (condition-ref e 'errno)))
	    ((condition-has-type? e <message-condition>) (print ":rename " (condition-ref e 'message)) (- ERANGE))
	    (else (- ERANGE)))
	 (your-rename from to)))

(define (call-link from to)
  (guard (e ((condition-has-type? e <system-error>) (- (condition-ref e 'errno)))
	    ((condition-has-type? e <message-condition>) (print ":link " (condition-ref e 'message)) (- ERANGE))
	    (else (- ERANGE)))
	 (your-link from to)))

(define (call-chmod path mode)
  (guard (e ((condition-has-type? e <system-error>) (- (condition-ref e 'errno)))
	    ((condition-has-type? e <message-condition>) (print ":chmod " (condition-ref e 'message)) (- ERANGE))
	    (else (- ERANGE)))
	 (your-chmod path mode)))

(define (call-chown path uid gid)
  (guard (e ((condition-has-type? e <system-error>) (- (condition-ref e 'errno)))
	    ((condition-has-type? e <message-condition>) (print ":chmod " (condition-ref e 'message)) (- ERANGE))
	    (else (- ERANGE)))
	 (your-chown path uid gid)))

(define (call-truncate path size)
  (guard (e ((condition-has-type? e <system-error>) (- (condition-ref e 'errno)))
	    ((condition-has-type? e <message-condition>) (print ":truncate " (condition-ref e 'message)) (- ERANGE))
	    (else (- ERANGE)))
	 (your-truncate path size)))

(define (call-open path file-info)
  (guard (e ((condition-has-type? e <system-error>) (- (condition-ref e 'errno)))
	    ((condition-has-type? e <message-condition>) (print ":open " (condition-ref e 'message)) (- ERANGE))
	    (else (- ERANGE)))
	 (your-open path file-info)))

(define (call-read path outp size offset file-info)
  (guard (e ((condition-has-type? e <system-error>) (- (condition-ref e 'errno)))
	    ((condition-has-type? e <message-condition>) (print ":read " (condition-ref e 'message)) (- ERANGE))
	    (else (- ERANGE)))
	 (your-read path outp size offset file-info)))

(define (call-write path inp size offset file-info)
  (guard (e ((condition-has-type? e <system-error>) (- (condition-ref e 'errno)))
	    ((condition-has-type? e <message-condition>) (print ":write " (condition-ref e 'message)) (- ERANGE))
	    (else (- ERANGE)))
	 (your-write path inp size offset file-info)))

(define (call-statfs path)
  (guard (e ((condition-has-type? e <system-error>) (- (condition-ref e 'errno)))
	    ((condition-has-type? e <message-condition>) (print ":statfs " (condition-ref e 'message)) (- ERANGE))
	    (else (- ERANGE)))
	 (your-statfs path)))

(define (call-flush path file-info)
  (guard (e ((condition-has-type? e <system-error>) (- (condition-ref e 'errno)))
	    ((condition-has-type? e <message-condition>) (print ":flush " (condition-ref e 'message)) (- ERANGE))
	    (else (- ERANGE)))
	 (your-flush path file-info)))

(define (call-release path file-info)
  (guard (e ((condition-has-type? e <system-error>) (- (condition-ref e 'errno)))
	    ((condition-has-type? e <message-condition>) (print ":release " (condition-ref e 'message)) (- ERANGE))
	    (else (- ERANGE)))
	 (your-release path file-info)))

(define (call-fsync path datasync? file-info)
  (guard (e ((condition-has-type? e <system-error>) (- (condition-ref e 'errno)))
	    ((condition-has-type? e <message-condition>) (print ":fsync " (condition-ref e 'message)) (- ERANGE))
	    (else (- ERANGE)))
	 (your-fsync path datasync? file-info)))

(define (call-setxattr path name value size flags)
  (guard (e ((condition-has-type? e <system-error>) (- (condition-ref e 'errno)))
	    ((condition-has-type? e <message-condition>) (print ":setxattr " (condition-ref e 'message)) (- ERANGE))
	    (else (- ERANGE)))
	 (your-setxattr path name value size flags)))

(define (call-getxattr path name value size)
  (guard (e ((condition-has-type? e <system-error>) (- (condition-ref e 'errno)))
	    ((condition-has-type? e <message-condition>) (print ":getxattr " (condition-ref e 'message)) (- ERANGE))
	    (else (- ERANGE)))
	 (your-getxattr path name value size)))

(define (call-listxattr path list size)
  (guard (e ((condition-has-type? e <system-error>) (- (condition-ref e 'errno)))
	    ((condition-has-type? e <message-condition>) (print ":listxattr " (condition-ref e 'message)) (- ERANGE))
	    (else (- ERANGE)))
	 (your-listxattr path list size)))

(define (call-removexattr path name)
  (guard (e ((condition-has-type? e <system-error>) (- (condition-ref e 'errno)))
	    ((condition-has-type? e <message-condition>) (print ":removexattr " (condition-ref e 'message)) (- ERANGE))
	    (else (- ERANGE)))
	 (your-removexattr path name)))

(define (call-opendir path file-info)
  (guard (e ((condition-has-type? e <system-error>) (- (condition-ref e 'errno)))
	    ((condition-has-type? e <message-condition>) (print ":opendir " (condition-ref e 'message)) (- ERANGE))
	    (else (- ERANGE)))
	 (your-opendir path file-info)))

(define (call-readdir path file-info)
  (guard (e ((condition-has-type? e <system-error>) (- (condition-ref e 'errno)))
	    ((condition-has-type? e <message-condition>) (print ":readdir " (condition-ref e 'message)) (- ERANGE))
	    (else (- ERANGE)))
	 (your-readdir path file-info)))

(define (call-releasedir path file-info)
  (guard (e ((condition-has-type? e <system-error>) (- (condition-ref e 'errno)))
	    ((condition-has-type? e <message-condition>) (print ":releasedir " (condition-ref e 'message)) (- ERANGE))
	    (else (- ERANGE)))
	 (your-releasedir path file-info)))

(define (call-fsyncdir path datasync? file-info)
  (guard (e ((condition-has-type? e <system-error>) (- (condition-ref e 'errno)))
	    ((condition-has-type? e <message-condition>) (print ":fsyncdir " (condition-ref e 'message)) (- ERANGE))
	    (else (- ERANGE)))
	 (your-fsyncdir path datasync? file-info)))

(define (call-init)
  (guard (e ((condition-has-type? e <message-condition>) (print ":init " (condition-ref e 'message))))
	 (your-init)))

(define (call-destroy)
  (guard (e ((condition-has-type? e <message-condition>) (print ":destroy " (condition-ref e 'message))))
	 (your-destroy)))

(define (call-access path mask)
  (guard (e ((condition-has-type? e <system-error>) (- (condition-ref e 'errno)))
	    ((condition-has-type? e <message-condition>) (print ":access " (condition-ref e 'message)) (- ERANGE))
	    (else (- ERANGE)))
	 (your-access path mask)))

(define (call-create path mode file-info)
  (guard (e ((condition-has-type? e <system-error>) (- (condition-ref e 'errno)))
	    ((condition-has-type? e <message-condition>) (print ":create " (condition-ref e 'message)) (- ERANGE))
	    (else (- ERANGE)))
	 (your-create path mode file-info)))

(define (call-ftruncate path size file-info)
  (guard (e ((condition-has-type? e <system-error>) (- (condition-ref e 'errno)))
	    ((condition-has-type? e <message-condition>) (print ":ftruncate " (condition-ref e 'message)) (- ERANGE))
	    (else (- ERANGE)))
	 (your-ftruncate path size file-info)))

(define (call-fgetattr path file-info)
  (guard (e ((condition-has-type? e <system-error>) (- (condition-ref e 'errno)))
	    ((condition-has-type? e <message-condition>) (print ":fgetattr " (condition-ref e 'message)) (- ERANGE))
	    (else (- ERANGE)))
	 (your-fgetattr path file-info)))

(define (call-utimens path atime mtime)
  (guard (e ((condition-has-type? e <system-error>) (- (condition-ref e 'errno)))
	    ((condition-has-type? e <message-condition>) (print ":utimens " (condition-ref e 'message)) (- ERANGE))
	    (else (- ERANGE)))
	 (your-utimens path atime mtime)))

(define callback-vector (vector call-getattr call-readlink call-mknod call-mkdir call-unlink call-rmdir call-symlink call-rename call-link call-chmod call-chown call-truncate call-open call-read call-write call-statfs call-flush call-release call-fsync call-setxattr call-getxattr call-listxattr call-removexattr call-opendir call-readdir call-releasedir call-fsyncdir call-init call-destroy call-access call-create call-ftruncate call-fgetattr call-utimens))

(define your-getattr #f)
(define your-readlink #f)
(define your-mknod #f)
(define your-mkdir #f)
(define your-unlink #f)
(define your-rmdir #f)
(define your-symlink #f)
(define your-rename #f)
(define your-link #f)
(define your-chmod #f)
(define your-chown #f)
(define your-truncate #f)
(define your-open #f)
(define your-read #f)
(define your-write #f)
(define your-statfs #f)
(define your-flush #f)
(define your-release #f)
(define your-fsync #f)
(define your-setxattr #f)
(define your-getxattr #f)
(define your-listxattr #f)
(define your-removexattr #f)
(define your-opendir #f)
(define your-readdir #f)
(define your-releasedir #f)
(define your-fsyncdir #f)
(define your-init #f)
(define your-destroy #f)
(define your-access #f)
(define your-create #f)
(define your-ftruncate #f)
(define your-fgetattr #f)
(define your-utimens #f)

(define (start-fuse mountpoint . operations)
  (let-keywords
   operations ((args (list "" "-f" "-s" "-d" mountpoint))
	       (multithreaded #f)
	       (getattr #f)
	       (readlink #f)
	       (mknod #f)
	       (mkdir #f)
	       (unlink #f)
	       (rmdir #f)
	       (symlink #f)
	       (rename #f)
	       (link #f)
	       (chmod #f)
	       (chown #f)
	       (truncate #f)
	       (open #f)
	       (read #f)
	       (write #f)
	       (statfs #f)
	       (flush #f)
	       (release #f)
	       (fsync #f)
	       (setxattr #f)
	       (getxattr #f)
	       (listxattr #f)
	       (removexattr #f)
	       (opendir #f)
	       (readdir #f)
	       (releasedir #f)
	       (fsyncdir #f)
	       (init #f)
	       (destroy #f)
	       (access #f)
	       (create #f)
	       (ftruncate #f)
	       (fgetattr #f)
	       (utimens #f))
   (and getattr (set! your-getattr getattr))
   (and readlink (set! your-readlink readlink))
   (and mknod (set! your-mknod mknod))
   (and mkdir (set! your-mkdir mkdir))
   (and unlink (set! your-unlink unlink))
   (and rmdir (set! your-rmdir rmdir))
   (and symlink (set! your-symlink symlink))
   (and rename (set! your-rename rename))
   (and link (set! your-link link))
   (and chmod (set! your-chmod chmod))
   (and chown (set! your-chown chown))
   (and truncate (set! your-truncate truncate))
   (and open (set! your-open open))
   (and read (set! your-read read))
   (and write (set! your-write write))
   (and statfs (set! your-statfs statfs))
   (and flush (set! your-flush flush))
   (and release (set! your-release release))
   (and fsync (set! your-fsync fsync))
   (and setxattr (set! your-setxattr setxattr))
   (and getxattr (set! your-getxattr getxattr))
   (and listxattr (set! your-listxattr listxattr))
   (and removexattr (set! your-removexattr removexattr))
   (and opendir (set! your-opendir opendir))
   (and readdir (set! your-readdir readdir))
   (and releasedir (set! your-releasedir releasedir))
   (and fsyncdir (set! your-fsyncdir fsyncdir))
   (and init (set! your-init init))
   (and destroy (set! your-destroy destroy))
   (and access (set! your-access access))
   (and create (set! your-create create))
   (and ftruncate (set! your-ftruncate ftruncate))
   (and fgetattr (set! your-fgetattr fgetattr))
   (and utimens (set! your-utimens utimens))
   (let1 f (fuse-mainsetup args
			   mountpoint
			   (boolean getattr)
			   (boolean readlink)
			   (boolean mknod)
			   (boolean mkdir)
			   (boolean unlink)
			   (boolean rmdir)
			   (boolean symlink)
			   (boolean rename)
			   (boolean link)
			   (boolean chmod)
			   (boolean chown)
			   (boolean truncate)
			   (boolean open)
			   (boolean read)
			   (boolean write)
			   (boolean statfs)
			   (boolean flush)
			   (boolean release)
			   (boolean fsync)
			   (boolean setxattr)
			   (boolean getxattr)
			   (boolean listxattr)
			   (boolean removexattr)
			   (boolean opendir)
			   (boolean readdir)
			   (boolean releasedir)
			   (boolean fsyncdir)
			   (boolean init)
			   (boolean destroy)
			   (boolean access)
			   (boolean create)
			   (boolean ftruncate)
			   (boolean fgetattr)
			   (boolean utimens))
     (when f 
       (set-signal-handler! SIGINT (lambda _ 
				     (for-each thread-terminate! thrds)
				     ;;(fuse-teardown f mountpoint)
				     (exit 1)))
       (fuse-mainloop f multithreaded)
       (fuse-teardown f mountpoint)))))

(define thrds (list))

(define (fuse-mainloop fuse multithreaded)
  (let loop ()
    (when (zero? (fuse-exited fuse))
      (let ((cmd (fuse-read-cmd fuse)))
	(if multithreaded
	  (let1 t (make-thread (lambda ()
				 (fuse-process-cmd fuse cmd)))
	    (push! thrds t)
	    (thread-start! t))
	  (fuse-process-cmd fuse cmd)))
      (loop))))

;; Epilogue
(provide "fuse")


