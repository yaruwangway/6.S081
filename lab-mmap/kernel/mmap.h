struct vma {
  uint64 addr;
  uint64 length;
  int prot;
  int flags;
  struct file *f;
  struct vma *next;
};
