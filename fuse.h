/*
 * fuse.h - Gauche FUSE binding
 *
 *   Copyright (c) 2008 Teruaki Gemma <teruakigemma@gmail.com>
 *
 *   Redistribution and use in source and binary forms, with or without
 *   modification, are permitted provided that the following conditions
 *   are met:
 * 
 *   1. Redistributions of source code must retain the above copyright
 *      notice, this list of conditions and the following disclaimer.
 *
 *   2. Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions and the following disclaimer in the
 *      documentation and/or other materials provided with the distribution.
 *
 *   3. Neither the name of the authors nor the names of its contributors
 *      may be used to endorse or promote products derived from this
 *      software without specific prior written permission.
 *
 *   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *   "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *   A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *   OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *   SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 *   TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 *   PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 *   LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 *   NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 *   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *  $Id: fuse.h 29 2008-11-23 18:50:11Z teruakigemma $
 */

/* Prologue */
#ifndef GAUCHE_FUSE_H
#define GAUCHE_FUSE_H

#define GAUCHE_API_0_9
#include <gauche.h>
#include <gauche/class.h>
#include <gauche/extend.h>
#define FUSE_USE_VERSION 26
#include <fuse.h>

SCM_DECL_BEGIN

/*
 * The following entry is a dummy one.
 * Replace it for your declarations.
 */

extern ScmObj test_fuse(void);
extern struct fuse* fuse_mainsetup(ScmObj args,
				   ScmObj mountpoint,
				   int getattr,
				   int readlink,
				   int mknod,
				   int mkdir,
				   int unlink,
				   int rmdir,
				   int symlink,
				   int rename,
				   int link,
				   int chmod,
				   int chown,
				   int truncate,
				   int open,
				   int read,
				   int write,
				   int statfs,
				   int flush,
				   int release,
				   int fsync,
				   int setxattr,
				   int getxattr,
				   int listxattr,
				   int removexattr,
				   int opendir,
				   int readdir,
				   int releasedir,
				   int fsyncdir,
				   int init,
				   int destroy,
				   int access,
				   int create,
				   int ftruncate,
				   int fgetattr,
				   int utimens);

extern ScmClass *FuseClass;
#define FUSE_P(obj)      SCM_XTYPEP(obj, FuseClass)
#define FUSE_UNBOX(obj)  SCM_FOREIGN_POINTER_REF(struct fuse*, obj)
#define FUSE_BOX(ptr)    Scm_MakeForeignPointer(FuseClass, ptr)

extern ScmClass *FuseCmdClass;
#define FUSECMD_P(obj)      SCM_XTYPEP(obj, FuseCmdClass)
#define FUSECMD_UNBOX(obj)  SCM_FOREIGN_POINTER_REF(struct fuse_cmd*, obj)
#define FUSECMD_BOX(ptr)    Scm_MakeForeignPointer(FuseCmdClass, ptr)

extern ScmClass *FuseFileInfoClass;
#define FUSEFILEINFO_P(obj)      SCM_XTYPEP(obj, FuseFileInfoClass)
#define FUSEFILEINFO_UNBOX(obj)  SCM_FOREIGN_POINTER_REF(struct fuse_file_info*, obj)
#define FUSEFILEINFO_BOX(ptr)    Scm_MakeForeignPointer(FuseFileInfoClass, ptr)

typedef struct ScmSysStatvfsRec {
    SCM_HEADER;
    struct statvfs statvfs;
} ScmSysStatvfs;

SCM_CLASS_DECL(Scm_SysStatvfsClass);
#define SCM_CLASS_SYS_STATVFS   (&Scm_SysStatvfsClass)
#define SCM_SYS_STATVFS(obj)    ((ScmSysStatvfs*)(obj))
#define SCM_SYS_STATVFS_P(obj)  (SCM_XTYPEP(obj, SCM_CLASS_SYS_STATVFS))

ScmObj Scm_MakeSysStatvfs(void);
ScmObj Scm_SysStatvfs(const char *path);

extern void Scm_Init_fuse();
/* Epilogue */
SCM_DECL_END

#endif  /* GAUCHE_FUSE_H */
