/*
 * fuse.c - C stub for FUSE.
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
 *  $Id: fuse.c 29 2008-11-23 18:50:11Z teruakigemma $
 */

#include "fuse.h"

ScmClass *FuseClass;
ScmClass *FuseCmdClass;
ScmClass *FuseFileInfoClass;

/*
 * The following function is a dummy one; replace it for
 * your C function definitions.
 */

ScmObj test_fuse(void)
{
    return SCM_MAKE_STR("fuse is working");
}

static void fuse_cleanup(ScmObj obj)
{
}

static void fusecmd_cleanup(ScmObj obj)
{
}

static void fusefileinfo_cleanup(ScmObj obj)
{
}

static ScmObj statvfs_allocate(ScmClass *klass, ScmObj initargs);

SCM_DEFINE_BUILTIN_CLASS(Scm_SysStatvfsClass,
                         NULL, NULL, NULL,
                         statvfs_allocate,
                         NULL);

static ScmObj statvfs_allocate(ScmClass *klass, ScmObj initargs)
{
    ScmSysStatvfs *s = SCM_NEW(ScmSysStatvfs);
    SCM_SET_CLASS(s, SCM_CLASS_SYS_STATVFS);
    memset(&s->statvfs, 0, sizeof(s->statvfs));
    return SCM_OBJ(s);
}

/* slot accessors */
#define STATVFS_GET_N_SET(name) \
  static ScmObj SCM_CPP_CAT3(statvfs_, name, _get)(ScmSysStatvfs* s)         \
  { return Scm_MakeIntegerFromUI((u_long)s->statvfs.name); }                    \
  static void SCM_CPP_CAT3(statvfs_, name, _set)(ScmSysStatvfs* s, ScmObj v) \
  {                                                                          \
      if (!SCM_INTEGERP(v)) Scm_Error("integer required, but got %S", v);    \
      s->statvfs.name = Scm_GetUInteger(v);                           \
  }

STATVFS_GET_N_SET(f_bsize)
STATVFS_GET_N_SET(f_frsize)
STATVFS_GET_N_SET(f_blocks)
STATVFS_GET_N_SET(f_bfree)
STATVFS_GET_N_SET(f_bavail)
STATVFS_GET_N_SET(f_files)
STATVFS_GET_N_SET(f_ffree)
STATVFS_GET_N_SET(f_favail)
STATVFS_GET_N_SET(f_fsid)
STATVFS_GET_N_SET(f_flag)
STATVFS_GET_N_SET(f_namemax)

static ScmClassStaticSlotSpec statvfs_slots[] = {
  SCM_CLASS_SLOT_SPEC("f_bsize", statvfs_f_bsize_get, statvfs_f_bsize_set),
  SCM_CLASS_SLOT_SPEC("f_frsize", statvfs_f_frsize_get, statvfs_f_frsize_set),
  SCM_CLASS_SLOT_SPEC("f_blocks", statvfs_f_blocks_get, statvfs_f_blocks_set),
  SCM_CLASS_SLOT_SPEC("f_bfree", statvfs_f_bfree_get, statvfs_f_bfree_set),
  SCM_CLASS_SLOT_SPEC("f_bavail", statvfs_f_bavail_get, statvfs_f_bavail_set),
  SCM_CLASS_SLOT_SPEC("f_files", statvfs_f_files_get, statvfs_f_files_set),
  SCM_CLASS_SLOT_SPEC("f_ffree", statvfs_f_ffree_get, statvfs_f_ffree_set),
  SCM_CLASS_SLOT_SPEC("f_favail", statvfs_f_favail_get, statvfs_f_favail_set),
  SCM_CLASS_SLOT_SPEC("f_fsid", statvfs_f_fsid_get, statvfs_f_fsid_set),
  SCM_CLASS_SLOT_SPEC("f_flag", statvfs_f_flag_get, statvfs_f_flag_set),
  SCM_CLASS_SLOT_SPEC("f_namemax", statvfs_f_namemax_get, statvfs_f_namemax_set),
  SCM_CLASS_SLOT_SPEC_END()
};

ScmObj Scm_MakeSysStatvfs(void)
{
  return statvfs_allocate(NULL, SCM_NIL);
}

ScmObj Scm_SysStatvfs(const char *path)
{
  ScmObj s = Scm_MakeSysStatvfs();
  int res;
  SCM_SYSCALL(res, statvfs(path, &(SCM_SYS_STATVFS(s)->statvfs)));
  if (res < 0) Scm_SysError("statvfs failed");
  return s;
}

static ScmVector *callback_vector = NULL;

static enum {
  CB_GETATTR,
  CB_READLINK,
  CB_MKNOD,
  CB_MKDIR,
  CB_UNLINK,
  CB_RMDIR,
  CB_SYMLINK,
  CB_RENAME,
  CB_LINK,
  CB_CHMOD,
  CB_CHOWN,
  CB_TRUNCATE,
  CB_OPEN,
  CB_READ,
  CB_WRITE,
  CB_STATFS,
  CB_FLUSH,
  CB_RELEASE,
  CB_FSYNC,
  CB_SETXATTR,
  CB_GETXATTR,
  CB_LISTXATTR,
  CB_REMOVEXATTR,
  CB_OPENDIR,
  CB_READDIR,
  CB_RELEASEDIR,
  CB_FSYNCDIR,
  CB_INIT,
  CB_DESTROY,
  CB_ACCESS,
  CB_CREATE,
  CB_FTRUNCATE,
  CB_FGETATTR,
  CB_UTIMENS
} callback_vector_index;

static int call_getattr(const char *path, struct stat *stbuf)
{
  ScmEvalPacket epak;
  Scm_Apply(SCM_VECTOR_ELEMENT(callback_vector, CB_GETATTR),
	    SCM_LIST1(SCM_MAKE_STR_COPYING(path)),
	    &epak);

  ScmObj res = epak.results[0];
  if (SCM_INTEGERP(res)) {
    if (Scm_Sign(res) == -1)
      return Scm_GetInteger(res);
  }
  if (SCM_SYS_STAT_P(res)) {
    *stbuf = *(&((ScmSysStat*)(res))->statrec);
  }
  return 0;

/* 	int res; */

/* 	res = lstat(path, stbuf); */
/* 	if (res == -1) */
/* 		return -errno; */

/* 	return 0; */
}

static int call_readlink(const char *path, char *buf, size_t size)
{
  ScmEvalPacket epak;
  Scm_Apply(SCM_VECTOR_ELEMENT(callback_vector, CB_READLINK),
	    SCM_LIST1(SCM_MAKE_STR_COPYING(path)),
	    &epak);

  ScmObj res = epak.results[0];
  if (SCM_INTEGERP(res)) {
    if (Scm_Sign(res) == -1)
      return Scm_GetInteger(res);
  }
  if (SCM_STRINGP(res)) {
    strncpy(buf,Scm_GetStringConst(SCM_STRING(res)),size);
    if (size > 0)
      buf[size-1] = '\0';
  }
  return 0;
/* 	int res; */

/* 	res = readlink(path, buf, size - 1); */
/* 	if (res == -1) */
/* 		return -errno; */

/* 	buf[res] = '\0'; */
/* 	return 0; */
}

static int call_mknod(const char *path, mode_t mode, dev_t rdev)
{
  ScmEvalPacket epak;
  Scm_Apply(SCM_VECTOR_ELEMENT(callback_vector, CB_MKNOD),
	    SCM_LIST3(SCM_MAKE_STR_COPYING(path),
		      Scm_MakeIntegerFromUI(mode),
		      Scm_MakeIntegerFromUI(rdev)),
	    &epak);
  ScmObj res = epak.results[0];
  if (SCM_INTEGERP(res)) {
    if (Scm_Sign(res) == -1)
      return Scm_GetInteger(res);
  }
  return 0;
/* 	int res; */

/* 	/\* On Linux this could just be 'mknod(path, mode, rdev)' but this */
/* 	   is more portable *\/ */
/* 	if (S_ISREG(mode)) { */
/* 		res = open(path, O_CREAT | O_EXCL | O_WRONLY, mode); */
/* 		if (res >= 0) */
/* 			res = close(res); */
/* 	} else if (S_ISFIFO(mode)) */
/* 		res = mkfifo(path, mode); */
/* 	else */
/* 		res = mknod(path, mode, rdev); */
/* 	if (res == -1) */
/* 		return -errno; */

/* 	return 0; */
}

static int call_mkdir(const char *path, mode_t mode)
{
  ScmEvalPacket epak;
  Scm_Apply(SCM_VECTOR_ELEMENT(callback_vector, CB_MKDIR),
	    SCM_LIST2(SCM_MAKE_STR_COPYING(path),
		      Scm_MakeIntegerFromUI(mode)),
	    &epak);
  ScmObj res = epak.results[0];
  if (SCM_INTEGERP(res)) {
    if (Scm_Sign(res) == -1)
      return Scm_GetInteger(res);
  }
  return 0;
/* 	int res; */

/* 	res = mkdir(path, mode); */
/* 	if (res == -1) */
/* 		return -errno; */

/* 	return 0; */
}

static int call_unlink(const char *path)
{
  ScmEvalPacket epak;
  Scm_Apply(SCM_VECTOR_ELEMENT(callback_vector, CB_UNLINK),
	    SCM_LIST1(SCM_MAKE_STR_COPYING(path)),
	    &epak);
  ScmObj res = epak.results[0];
  if (SCM_INTEGERP(res)) {
    if (Scm_Sign(res) == -1)
      return Scm_GetInteger(res);
  }
  return 0;
/* 	int res; */

/* 	res = unlink(path); */
/* 	if (res == -1) */
/* 		return -errno; */

/* 	return 0; */
}

static int call_rmdir(const char *path)
{
  ScmEvalPacket epak;
  Scm_Apply(SCM_VECTOR_ELEMENT(callback_vector, CB_RMDIR),
	    SCM_LIST1(SCM_MAKE_STR_COPYING(path)),
	    &epak);
  ScmObj res = epak.results[0];
  if (SCM_INTEGERP(res)) {
    if (Scm_Sign(res) == -1)
      return Scm_GetInteger(res);
  }
  return 0;
/* 	int res; */

/* 	res = rmdir(path); */
/* 	if (res == -1) */
/* 		return -errno; */

/* 	return 0; */
}

static int call_symlink(const char *from, const char *to)
{
  ScmEvalPacket epak;
  Scm_Apply(SCM_VECTOR_ELEMENT(callback_vector, CB_SYMLINK),
	    SCM_LIST2(SCM_MAKE_STR_COPYING(from),
		      SCM_MAKE_STR_COPYING(to)),
	    &epak);
  ScmObj res = epak.results[0];
  if (SCM_INTEGERP(res)) {
    if (Scm_Sign(res) == -1)
      return Scm_GetInteger(res);
  }
  return 0;
/* 	int res; */

/* 	res = symlink(from, to); */
/* 	if (res == -1) */
/* 		return -errno; */

/* 	return 0; */
}

static int call_rename(const char *from, const char *to)
{
  ScmEvalPacket epak;
  Scm_Apply(SCM_VECTOR_ELEMENT(callback_vector, CB_RENAME),
	    SCM_LIST2(SCM_MAKE_STR_COPYING(from),
		      SCM_MAKE_STR_COPYING(to)),
	    &epak);
  ScmObj res = epak.results[0];
  if (SCM_INTEGERP(res)) {
    if (Scm_Sign(res) == -1)
      return Scm_GetInteger(res);
  }
  return 0;
/* 	int res; */

/* 	res = rename(from, to); */
/* 	if (res == -1) */
/* 		return -errno; */

/* 	return 0; */
}

static int call_link(const char *from, const char *to)
{
  ScmEvalPacket epak;
  Scm_Apply(SCM_VECTOR_ELEMENT(callback_vector, CB_LINK),
	    SCM_LIST2(SCM_MAKE_STR_COPYING(from),
		      SCM_MAKE_STR_COPYING(to)),
	    &epak);
  ScmObj res = epak.results[0];
  if (SCM_INTEGERP(res)) {
    if (Scm_Sign(res) == -1)
      return Scm_GetInteger(res);
  }
  return 0;
/* 	int res; */

/* 	res = link(from, to); */
/* 	if (res == -1) */
/* 		return -errno; */

/* 	return 0; */
}

static int call_chmod(const char *path, mode_t mode)
{
  ScmEvalPacket epak;
  Scm_Apply(SCM_VECTOR_ELEMENT(callback_vector, CB_CHMOD),
	    SCM_LIST2(SCM_MAKE_STR_COPYING(path),
		      Scm_MakeIntegerFromUI(mode)),
	    &epak);
  ScmObj res = epak.results[0];
  if (SCM_INTEGERP(res)) {
    if (Scm_Sign(res) == -1)
      return Scm_GetInteger(res);
  }
  return 0;

/* 	int res; */

/* 	res = chmod(path, mode); */
/* 	if (res == -1) */
/* 		return -errno; */

/* 	return 0; */
}

static int call_chown(const char *path, uid_t uid, gid_t gid)
{
  ScmEvalPacket epak;
  Scm_Apply(SCM_VECTOR_ELEMENT(callback_vector, CB_CHOWN),
	    SCM_LIST3(SCM_MAKE_STR_COPYING(path),
		      Scm_MakeIntegerFromUI(uid),
		      Scm_MakeIntegerFromUI(gid)),
	    &epak);
  ScmObj res = epak.results[0];
  if (SCM_INTEGERP(res)) {
    if (Scm_Sign(res) == -1)
      return Scm_GetInteger(res);
  }
  return 0;

/* 	int res; */

/* 	res = lchown(path, uid, gid); */
/* 	if (res == -1) */
/* 		return -errno; */

/* 	return 0; */
}

static int call_truncate(const char *path, off_t size)
{
  ScmEvalPacket epak;
  Scm_Apply(SCM_VECTOR_ELEMENT(callback_vector, CB_TRUNCATE),
	    SCM_LIST2(SCM_MAKE_STR_COPYING(path),
		      Scm_OffsetToInteger(size)),
	    &epak);
  ScmObj res = epak.results[0];
  if (SCM_INTEGERP(res)) {
    if (Scm_Sign(res) == -1)
      return Scm_GetInteger(res);
  }
  return 0;
/* 	int res; */

/* 	res = truncate(path, size); */
/* 	if (res == -1) */
/* 		return -errno; */

/* 	return 0; */
}

static int call_open(const char *path, struct fuse_file_info *fi)
{
  ScmEvalPacket epak;
  Scm_Apply(SCM_VECTOR_ELEMENT(callback_vector, CB_OPEN),
	    SCM_LIST2(SCM_MAKE_STR_COPYING(path),
		      FUSEFILEINFO_BOX(fi)),
	    &epak);
  ScmObj res = epak.results[0];
  if (SCM_INTEGERP(res)) {
    if (Scm_Sign(res) == -1)
      return Scm_GetInteger(res);
  }
  return 0;
/* 	int res; */

/* 	res = open(path, fi->flags); */
/* 	if (res == -1) */
/* 		return -errno; */

/* 	close(res); */
/* 	return 0; */
}

static int call_read(const char *path, char *buf, size_t size, off_t offset,
		    struct fuse_file_info *fi)
{
  ScmEvalPacket epak;
  ScmObj outp = Scm_MakeOutputStringPort(TRUE);
  Scm_Apply(SCM_VECTOR_ELEMENT(callback_vector, CB_READ),
	    SCM_LIST5(SCM_MAKE_STR_COPYING(path),
		      outp,
		      Scm_OffsetToInteger(size),
		      Scm_OffsetToInteger(offset),
		      FUSEFILEINFO_BOX(fi)),
	    &epak);
  Scm_ClosePort(SCM_PORT(outp));
  ScmObj res = epak.results[0];
  if (SCM_INTEGERP(res)) {
    if (Scm_Sign(res) == -1)
      return Scm_GetInteger(res);
  }
  char *src = Scm_GetString(SCM_STRING(Scm_GetOutputString(SCM_PORT(outp),0)));
  memcpy(buf, src, size);
  if (Scm_IntegerP(res))
    return Scm_GetInteger(res);
  /* never reach */
  return -1;
/* 	int fd; */
/* 	int res; */

/* 	(void) fi; */
/* 	fd = open(path, O_RDONLY); */
/* 	if (fd == -1) */
/* 		return -errno; */

/* 	res = read(fd, buf, size); */
/* 	if (res == -1) */
/* 		res = -errno; */

/* 	close(fd); */
/* 	return res; */
}

static int call_write(const char *path, const char *buf, size_t size,
		     off_t offset, struct fuse_file_info *fi)
{
  ScmEvalPacket epak;
  ScmObj inp = Scm_MakeInputStringPort(SCM_STRING(Scm_MakeString(buf,size,-1,SCM_STRING_COPYING)),TRUE);
  Scm_Apply(SCM_VECTOR_ELEMENT(callback_vector, CB_WRITE),
	    SCM_LIST5(SCM_MAKE_STR_COPYING(path),
		      inp,
		      Scm_OffsetToInteger(size),
		      Scm_OffsetToInteger(offset),
		      FUSEFILEINFO_BOX(fi)),
	    &epak);
  Scm_ClosePort(SCM_PORT(inp));
  ScmObj res = epak.results[0];
  if (SCM_INTEGERP(res)) {
    if (Scm_Sign(res) == -1)
      return Scm_GetInteger(res);
  }
  if (Scm_IntegerP(res))
    return Scm_GetInteger(res);
  /* never reach */
  return -1;

/* 	int fd; */
/* 	int res; */

/* 	(void) fi; */
/* 	fd = open(path, O_WRONLY); */
/* 	if (fd == -1) */
/* 		return -errno; */

/* 	res = write(fd, buf, size); */
/* 	if (res == -1) */
/* 		res = -errno; */

/* 	close(fd); */
/* 	return res; */
}

static int call_statfs(const char *path, struct statvfs *stbuf)
{
  ScmEvalPacket epak;
  Scm_Apply(SCM_VECTOR_ELEMENT(callback_vector, CB_STATFS),
	    SCM_LIST1(SCM_MAKE_STR_COPYING(path)),
	    &epak);

  ScmObj res = epak.results[0];
  if (SCM_INTEGERP(res)) {
    if (Scm_Sign(res) == -1)
      return Scm_GetInteger(res);
  }
  if (SCM_SYS_STATVFS_P(res)) {
    *stbuf = SCM_SYS_STATVFS(res)->statvfs;
    return 0;
  }
  return -ENOENT;

/* 	int res; */

/* 	res = statvfs(path, stbuf); */
/* 	if (res == -1) */
/* 		return -errno; */

/* 	return 0; */
}

static int call_flush(const char *path, struct fuse_file_info *fi)
{
  ScmEvalPacket epak;
  Scm_Apply(SCM_VECTOR_ELEMENT(callback_vector, CB_FLUSH),
	    SCM_LIST2(SCM_MAKE_STR_COPYING(path),
		      FUSEFILEINFO_BOX(fi)),
	    &epak);
  ScmObj res = epak.results[0];
  if (SCM_INTEGERP(res)) {
    if (Scm_Sign(res) == -1)
      return Scm_GetInteger(res);
  }
  return 0;
}

static int call_release(const char *path, struct fuse_file_info *fi)
{
  ScmEvalPacket epak;
  Scm_Apply(SCM_VECTOR_ELEMENT(callback_vector, CB_RELEASE),
	    SCM_LIST2(SCM_MAKE_STR_COPYING(path),
		      FUSEFILEINFO_BOX(fi)),
	    &epak);
  ScmObj res = epak.results[0];
  if (SCM_INTEGERP(res)) {
    if (Scm_Sign(res) == -1)
      return Scm_GetInteger(res);
  }
  return 0;

/* 	/\* Just a stub.	 This method is optional and can safely be left */
/* 	   unimplemented *\/ */

/* 	(void) path; */
/* 	(void) fi; */
/* 	return 0; */
}

static int call_fsync(const char *path, int isdatasync, struct fuse_file_info *fi)
{
  ScmEvalPacket epak;
  Scm_Apply(SCM_VECTOR_ELEMENT(callback_vector, CB_FSYNC),
	    SCM_LIST3(SCM_MAKE_STR_COPYING(path),
		      SCM_MAKE_BOOL(isdatasync),
		      FUSEFILEINFO_BOX(fi)),
	    &epak);
  ScmObj res = epak.results[0];
  if (SCM_INTEGERP(res)) {
    if (Scm_Sign(res) == -1)
      return Scm_GetInteger(res);
  }
  return 0;
/* 	/\* Just a stub.	 This method is optional and can safely be left */
/* 	   unimplemented *\/ */

/* 	(void) path; */
/* 	(void) isdatasync; */
/* 	(void) fi; */
/* 	return 0; */
}


static int call_setxattr(const char *path, const char *name, const char *value, size_t size, int flags)
{
  ScmEvalPacket epak;
  Scm_Apply(SCM_VECTOR_ELEMENT(callback_vector, CB_SETXATTR),
	    SCM_LIST4(SCM_MAKE_STR_COPYING(path),
		      SCM_MAKE_STR_COPYING(name),
		      SCM_MAKE_STR_COPYING(value),
		      Scm_MakeInteger(flags)),
	    &epak);
  ScmObj res = epak.results[0];
  if (SCM_INTEGERP(res)) {
    if (Scm_Sign(res) == -1)
      return Scm_GetInteger(res);
  }
  return 0;
}

static int call_getxattr(const char *path, const char *name, char *value, size_t size)
{
  ScmEvalPacket epak;
  Scm_Apply(SCM_VECTOR_ELEMENT(callback_vector, CB_GETXATTR),
	    SCM_LIST2(SCM_MAKE_STR_COPYING(path),
		      SCM_MAKE_STR_COPYING(name)),
	    &epak);
  ScmObj res = epak.results[0];
  if (SCM_INTEGERP(res)) {
    if (Scm_Sign(res) == -1)
      return Scm_GetInteger(res);
  }
  if (SCM_STRINGP(res)) {
    int psize = 0;
    const char *str = Scm_GetStringContent(SCM_STRING(res), &psize, NULL, NULL);
    if (0 == size) {
      return psize;
    } else {
      strncpy(value,str,size);
      value[size-1] = '\0';
      return strlen(value);
    }
  }
  return size;  
}

static int call_listxattr(const char *path, char *list, size_t size)
{
  ScmEvalPacket epak;
  Scm_Apply(SCM_VECTOR_ELEMENT(callback_vector, CB_LISTXATTR),
	    SCM_LIST1(SCM_MAKE_STR_COPYING(path)),
	    &epak);
  ScmObj res = epak.results[0];
  if (SCM_INTEGERP(res)) {
    if (Scm_Sign(res) == -1)
      return Scm_GetInteger(res);
  } 
  if (SCM_LISTP(res)) {
    int i;
    int len = Scm_Length(res);
    int s = 0;
    char **strs = Scm_ListToCStringArray(res,TRUE,NULL);
    if (0 == size) {
      for (i=0; i<len; i++)
	s += strlen(strs[i]);
      return s;
    } else {      
      for (i=0; i<len; i++)
	strncat(list, strs[i], size - strlen(list) - 1);
      return strlen(list);
    };
  }
  return 0;
}

static int call_removexattr(const char *path, const char *name)
{
  ScmEvalPacket epak;
  Scm_Apply(SCM_VECTOR_ELEMENT(callback_vector, CB_REMOVEXATTR),
	    SCM_LIST2(SCM_MAKE_STR_COPYING(path),
		      SCM_MAKE_STR_COPYING(name)),
	    &epak);
  ScmObj res = epak.results[0];
  if (SCM_INTEGERP(res)) {
    if (Scm_Sign(res) == -1)
      return Scm_GetInteger(res);
  }
  return 0;  
}

static int call_opendir(const char *path, struct fuse_file_info *fi)
{
  ScmEvalPacket epak;
  Scm_Apply(SCM_VECTOR_ELEMENT(callback_vector, CB_OPENDIR),
	    SCM_LIST2(SCM_MAKE_STR_COPYING(path),
		      FUSEFILEINFO_BOX(fi)),
	    &epak);
  ScmObj res = epak.results[0];
  if (SCM_INTEGERP(res)) {
    if (Scm_Sign(res) == -1)
      return Scm_GetInteger(res);
  }
  return 0;  
}

static int call_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
		       off_t offset, struct fuse_file_info *fi)
{
  ScmEvalPacket epak;
  Scm_Apply(SCM_VECTOR_ELEMENT(callback_vector, CB_READDIR),
            SCM_LIST2(SCM_MAKE_STR_COPYING(path),
		      FUSEFILEINFO_BOX(fi)),
            &epak);
  ScmObj res = epak.results[0];
  if (SCM_INTEGERP(res)) {
    if (Scm_Sign(res) == -1)
      return Scm_GetInteger(res);
  }
  if (SCM_LISTP(res)) {
    int i;
    int len = Scm_Length(res);
    char **strs = Scm_ListToCStringArray(res,TRUE,NULL);
    for (i=0; i<len; i++)
      filler(buf, strs[i], NULL, 0);
  }
  return 0;
  
  /* note: #include <dirent.h> */
  /*      DIR *dp; */
  /*      struct dirent *de; */
  
  /*      (void) offset; */
  /*      (void) fi; */
  
  /*      dp = opendir(path); */
  /*      if (dp == NULL) */
  /*              return -errno; */
  
  /*      while ((de = readdir(dp)) != NULL) { */
  /*              struct stat st; */
  /*              memset(&st, 0, sizeof(st)); */
  /*              st.st_ino = de->d_ino; */
  /*              st.st_mode = de->d_type << 12; */
  /*              if (filler(buf, de->d_name, &st, 0)) */
  /*                      break; */
  /*      } */
  
  /*      closedir(dp); */
  /*      return 0; */
}

static int call_releasedir(const char *path, struct fuse_file_info *fi)
{
  ScmEvalPacket epak;
  Scm_Apply(SCM_VECTOR_ELEMENT(callback_vector, CB_RELEASEDIR),
	    SCM_LIST2(SCM_MAKE_STR_COPYING(path),
		      FUSEFILEINFO_BOX(fi)),
	    &epak);
  ScmObj res = epak.results[0];
  if (SCM_INTEGERP(res)) {
    if (Scm_Sign(res) == -1)
      return Scm_GetInteger(res);
  }
  return 0;  
}

static int call_fsyncdir(const char *path, int datasync, struct fuse_file_info *fi)
{
  ScmEvalPacket epak;
  Scm_Apply(SCM_VECTOR_ELEMENT(callback_vector, CB_FSYNCDIR),
	    SCM_LIST3(SCM_MAKE_STR_COPYING(path),
		      SCM_MAKE_BOOL(datasync),
		      FUSEFILEINFO_BOX(fi)),
	    &epak);
  ScmObj res = epak.results[0];
  if (SCM_INTEGERP(res)) {
    if (Scm_Sign(res) == -1)
      return Scm_GetInteger(res);
  }
  return 0;  
}

void *call_init(struct fuse_conn_info *conn)
{
  ScmEvalPacket epak;
  Scm_Apply(SCM_VECTOR_ELEMENT(callback_vector, CB_INIT),
	    SCM_NIL,
	    &epak);
}

void call_destroy(void *data)
{
  ScmEvalPacket epak;
  Scm_Apply(SCM_VECTOR_ELEMENT(callback_vector, CB_DESTROY),
	    SCM_NIL,
	    &epak);
}

static int call_access(const char *path, int mask)
{
  ScmEvalPacket epak;
  Scm_Apply(SCM_VECTOR_ELEMENT(callback_vector, CB_ACCESS),
	    SCM_LIST2(SCM_MAKE_STR_COPYING(path),
		      SCM_MAKE_INT(mask)),
	    &epak);

  ScmObj res = epak.results[0];
  if (SCM_INTEGERP(res)) {
    if (Scm_Sign(res) == -1)
      return Scm_GetInteger(res);
  }
  if (SCM_FALSEP(res))
    return -EACCES;
  return 0;
/* 	int res; */

/* 	res = access(path, mask); */
/* 	if (res == -1) */
/* 		return -errno; */

/* 	return 0; */
}

static int call_create(const char *path, mode_t mode, struct fuse_file_info *fi)
{
  ScmEvalPacket epak;
  Scm_Apply(SCM_VECTOR_ELEMENT(callback_vector, CB_CREATE),
	    SCM_LIST3(SCM_MAKE_STR_COPYING(path),
		      Scm_MakeIntegerFromUI(mode),
		      FUSEFILEINFO_BOX(fi)),
	    &epak);
  ScmObj res = epak.results[0];
  if (SCM_INTEGERP(res)) {
    if (Scm_Sign(res) == -1)
      return Scm_GetInteger(res);
  }
  return 0;  
}

static int call_ftruncate(const char *path, off_t size, struct fuse_file_info *fi)
{
  ScmEvalPacket epak;
  Scm_Apply(SCM_VECTOR_ELEMENT(callback_vector, CB_FTRUNCATE),
	    SCM_LIST3(SCM_MAKE_STR_COPYING(path),
		      Scm_OffsetToInteger(size), 
		      FUSEFILEINFO_BOX(fi)),
	    &epak);
  ScmObj res = epak.results[0];
  if (SCM_INTEGERP(res)) {
    if (Scm_Sign(res) == -1)
      return Scm_GetInteger(res);
  }
  return 0;  
}

static int call_fgetattr(const char *path, struct stat *buf, struct fuse_file_info *fi)
{
  ScmEvalPacket epak;
  Scm_Apply(SCM_VECTOR_ELEMENT(callback_vector, CB_FGETATTR),
	    SCM_LIST2(SCM_MAKE_STR_COPYING(path),
		      FUSEFILEINFO_BOX(fi)),
	    &epak);

  ScmObj res = epak.results[0];
  if (SCM_INTEGERP(res)) {
    if (Scm_Sign(res) == -1)
      return Scm_GetInteger(res);
  }
  if (SCM_SYS_STAT_P(res)) {
    *buf = *(&((ScmSysStat*)(res))->statrec);
  }
  return 0;
}

static int call_utimens(const char *path, const struct timespec ts[2])
{
  ScmEvalPacket epak;
  Scm_Apply(SCM_VECTOR_ELEMENT(callback_vector, CB_UTIMENS),
	    SCM_LIST3(SCM_MAKE_STR_COPYING(path),
		      Scm_MakeSysTime(ts[0].tv_sec),
		      Scm_MakeSysTime(ts[1].tv_sec)),
	    &epak);
  ScmObj res = epak.results[0];
  if (SCM_INTEGERP(res)) {
    if (Scm_Sign(res) == -1)
      return Scm_GetInteger(res);
  }
  return 0;

/* 	int res; */
/* 	struct timeval tv[2]; */

/* 	tv[0].tv_sec = ts[0].tv_sec; */
/* 	tv[0].tv_usec = ts[0].tv_nsec / 1000; */
/* 	tv[1].tv_sec = ts[1].tv_sec; */
/* 	tv[1].tv_usec = ts[1].tv_nsec / 1000; */

/* 	res = utimes(path, tv); */
/* 	if (res == -1) */
/* 		return -errno; */

/* 	return 0; */
}

static struct fuse_operations fuse_oper;

struct fuse* fuse_mainsetup(ScmObj args,
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
			    int utimens)
{
  int multithreaded;
  int fd;
  int argc = Scm_Length(args);
  char **argv = Scm_ListToCStringArray(args,TRUE,NULL);
  char *mp = Scm_GetString(SCM_STRING(mountpoint));
  callback_vector = SCM_VECTOR(Scm_GlobalVariableRef(SCM_FIND_MODULE("fuse",FALSE),
						     SCM_SYMBOL(SCM_INTERN("callback-vector")),0));
  if (getattr) fuse_oper.getattr = call_getattr;
  if (readlink) fuse_oper.readlink = call_readlink;
  if (mknod) fuse_oper.mknod = call_mknod;
  if (mkdir) fuse_oper.mkdir = call_mkdir;
  if (unlink) fuse_oper.unlink = call_unlink;
  if (rmdir) fuse_oper.rmdir = call_rmdir;
  if (symlink) fuse_oper.symlink = call_symlink;
  if (rename) fuse_oper.rename = call_rename;
  if (link) fuse_oper.link = call_link;
  if (chmod) fuse_oper.chmod = call_chmod;
  if (chown) fuse_oper.chown = call_chown;
  if (truncate) fuse_oper.truncate = call_truncate;
  if (open) fuse_oper.open = call_open;
  if (read) fuse_oper.read = call_read;
  if (write) fuse_oper.write = call_write;
  if (statfs) fuse_oper.statfs = call_statfs;
  if (flush) fuse_oper.flush = call_flush;
  if (release) fuse_oper.release = call_release;
  if (fsync) fuse_oper.fsync = call_fsync;
  if (setxattr) fuse_oper.setxattr = call_setxattr;
  if (getxattr) fuse_oper.getxattr = call_getxattr;
  if (listxattr) fuse_oper.listxattr = call_listxattr;
  if (removexattr) fuse_oper.removexattr = call_removexattr;
  if (opendir) fuse_oper.opendir = call_opendir;
  if (readdir) fuse_oper.readdir = call_readdir;
  if (releasedir) fuse_oper.releasedir = call_releasedir;
  if (fsyncdir) fuse_oper.fsyncdir = call_fsyncdir;
  if (init) fuse_oper.init = call_init;
  if (destroy) fuse_oper.destroy = call_destroy;
  if (access) fuse_oper.access = call_access;
  if (create) fuse_oper.create = call_create;
  if (ftruncate) fuse_oper.ftruncate = call_ftruncate;
  if (fgetattr) fuse_oper.fgetattr = call_fgetattr;
  if (utimens) fuse_oper.utimens = call_utimens;
  struct fuse * fuse = fuse_setup(argc,argv,&fuse_oper,sizeof(struct fuse_operations),&mp,&multithreaded,&fd);
  if (fuse!=NULL) {
    return fuse;
  };
  return NULL;
}

/*
 * Module initialization function.
 */
extern void Scm_Init_fuselib(ScmModule*);

void Scm_Init_fuse(void)
{
    ScmModule *mod;

    /* Register this DSO to Gauche */
    SCM_INIT_EXTENSION(fuse);

    /* Create the module if it doesn't exist yet. */
    mod = SCM_MODULE(SCM_FIND_MODULE("fuse", TRUE));

    FuseClass =
      Scm_MakeForeignPointerClass(mod, "<fuse>",
				  NULL,
				  fuse_cleanup,
				  SCM_FOREIGN_POINTER_KEEP_IDENTITY|SCM_FOREIGN_POINTER_MAP_NULL);
    FuseCmdClass =
      Scm_MakeForeignPointerClass(mod, "<fuse-cmd>",
				    NULL,
				    fusecmd_cleanup,
                                    SCM_FOREIGN_POINTER_KEEP_IDENTITY|SCM_FOREIGN_POINTER_MAP_NULL);
    FuseFileInfoClass =
      Scm_MakeForeignPointerClass(mod, "<fuse-file-info>",
				  NULL,
				  fusefileinfo_cleanup,
				  SCM_FOREIGN_POINTER_KEEP_IDENTITY|SCM_FOREIGN_POINTER_MAP_NULL);

    Scm_InitStaticClass(&Scm_SysStatvfsClass, "<sys-statvfs>", mod,
                        statvfs_slots, 0);

    /* Register stub-generated procedures */
    Scm_Init_fuselib(mod);
}

