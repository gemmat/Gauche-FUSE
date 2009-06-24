;;;
;;; test.scm - test for fuse module
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
;;;  $Id: test.scm 29 2008-11-23 18:50:11Z teruakigemma $
;;;

(use gauche.test)

(test-start "fuse")
(use fuse)
(test-module 'fuse)

;; The following is a dummy test code.
;; Replace it for your tests.
(test* "test-fuse" "fuse is working"
       (test-fuse))

(define sys-statvfs (sys-statvfs "/"))

(test* "<sys-statvfs> slot 'f_bsize should not be zero"
       (slot-ref sys-statvfs 'f_bsize)
       (slot-ref sys-statvfs 'f_bsize)
       (lambda (_ result-of-thunk)
	 (not (zero? result-of-thunk))))

(test* "<sys-statvfs> slot 'f_frsize should not be zero"
       (slot-ref sys-statvfs 'f_frsize)
       (slot-ref sys-statvfs 'f_frsize)
       (lambda (_ result-of-thunk)
	 (not (zero? result-of-thunk))))

(test* "<sys-statvfs> slot 'f_blocks should not be zero"
       (slot-ref sys-statvfs 'f_blocks)
       (slot-ref sys-statvfs 'f_blocks)
       (lambda (_ result-of-thunk)
	 (not (zero? result-of-thunk))))

(test* "<sys-statvfs> slot 'f_bfree should not be zero"
       (slot-ref sys-statvfs 'f_bfree)
       (slot-ref sys-statvfs 'f_bfree)
       (lambda (_ result-of-thunk)
	 (not (zero? result-of-thunk))))

(test* "<sys-statvfs> slot 'f_bavail should not be zero"
       (slot-ref sys-statvfs 'f_bavail)
       (slot-ref sys-statvfs 'f_bavail)
       (lambda (_ result-of-thunk)
	 (not (zero? result-of-thunk))))

(test* "<sys-statvfs> slot 'f_files should not be zero"
       (slot-ref sys-statvfs 'f_files)
       (slot-ref sys-statvfs 'f_files)
       (lambda (_ result-of-thunk)
	 (not (zero? result-of-thunk))))

(test* "<sys-statvfs> slot 'f_ffree should not be zero"
       (slot-ref sys-statvfs 'f_ffree)
       (slot-ref sys-statvfs 'f_ffree)
       (lambda (_ result-of-thunk)
	 (not (zero? result-of-thunk))))

(test* "<sys-statvfs> slot 'f_favail should not be zero"
       (slot-ref sys-statvfs 'f_favail)
       (slot-ref sys-statvfs 'f_favail)
       (lambda (_ result-of-thunk)
	 (not (zero? result-of-thunk))))

(test* "<sys-statvfs> slot 'f_fsid should not be zero"
       (slot-ref sys-statvfs 'f_fsid)
       (slot-ref sys-statvfs 'f_fsid)
       (lambda (_ result-of-thunk)
	 (not (zero? result-of-thunk))))

(test* "<sys-statvfs> slot 'f_flag should not be zero"
       (slot-ref sys-statvfs 'f_flag)
       (slot-ref sys-statvfs 'f_flag)
       (lambda (_ result-of-thunk)
	 (not (zero? result-of-thunk))))

(test* "<sys-statvfs> slot 'f_namemax should not be zero"
       (slot-ref sys-statvfs 'f_namemax)
       (slot-ref sys-statvfs 'f_namemax)
       (lambda (_ result-of-thunk)
	 (not (zero? result-of-thunk))))

(define st (sys-stat "/"))

(test "overwrite <sys-stat> slot 'type"
      (slot-ref st 'type)
      (lambda ()
	(sys-stat->file-type-set! st (slot-ref st 'type))
	(slot-ref st 'type)))

(test "overwrite <sys-stat> slot 'perm"
      (slot-ref st 'perm)
      (lambda ()
	(sys-stat->perm-set! st (slot-ref st 'perm))
	(slot-ref st 'perm)))

(test "overwrite <sys-stat> slot 'mode"
      (slot-ref st 'mode)
      (lambda ()
	(sys-stat->mode-set! st (slot-ref st 'mode))
	(slot-ref st 'mode)))

(test "overwrite <sys-stat> slot 'ino"
      (slot-ref st 'ino)
      (lambda ()
	(sys-stat->ino-set! st (slot-ref st 'ino))
	(slot-ref st 'ino)))

(test "overwrite <sys-stat> slot 'dev"
      (slot-ref st 'dev)
      (lambda ()
	(sys-stat->dev-set! st (slot-ref st 'dev))
	(slot-ref st 'dev)))

(test "overwrite <sys-stat> slot 'rdev"
      (slot-ref st 'rdev)
      (lambda ()
	(sys-stat->rdev-set! st (slot-ref st 'rdev))
	(slot-ref st 'rdev)))

(test "overwrite <sys-stat> slot 'nlink"
      (slot-ref st 'nlink)
      (lambda ()
	(sys-stat->nlink-set! st (slot-ref st 'nlink))
	(slot-ref st 'nlink)))

(test "overwrite <sys-stat> slot 'uid"
      (slot-ref st 'uid)
      (lambda ()
	(sys-stat->uid-set! st (slot-ref st 'uid))
	(slot-ref st 'uid)))

(test "overwrite <sys-stat> slot 'gid"
      (slot-ref st 'gid)
      (lambda ()
	(sys-stat->gid-set! st (slot-ref st 'gid))
	(slot-ref st 'gid)))

(test "overwrite <sys-stat> slot 'size"
      (slot-ref st 'size)
      (lambda ()
	(sys-stat->size-set! st (slot-ref st 'size))
	(slot-ref st 'size)))

(test "overwrite <sys-stat> slot 'atime"
      (slot-ref st 'atime)
      (lambda ()
	(sys-stat->atime-set! st (slot-ref st 'atime))
	(slot-ref st 'atime)))

(test "overwrite <sys-stat> slot 'mtime"
      (slot-ref st 'mtime)
      (lambda ()
	(sys-stat->mtime-set! st (slot-ref st 'mtime))
	(slot-ref st 'mtime)))

(test "overwrite <sys-stat> slot 'ctime"
      (slot-ref st 'ctime)
      (lambda ()
	(sys-stat->ctime-set! st (slot-ref st 'ctime))
	(slot-ref st 'ctime)))

;; epilogue
(test-end)
