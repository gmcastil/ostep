# Operating Systems Self-Study Plan

## Overview
This is a **20-week self-study plan** focused on **hands-on programming and projects** using *Operating Systems: Three Easy Pieces (OSTEP)* and **xv6**. The emphasis is on **practical implementation** over reading, with minimal theory.

## Weekly Breakdown

### **Weeks 1-2: Foundations & Setup**
**Goals**: Set up the environment, refresh C, and practice basic systems programming.
- **Read**: Ch. 5 (*Process API*)
- **Projects**:
  - Implement a **simple shell** (process creation, fork/exec/wait).
- **Expected Time Commitment**: ~10-12 hours/week.

---

### **Weeks 3-4: Process Scheduling**
**Goals**: Learn process scheduling and implement a scheduler.
- **Read**:
  - Ch. 6 (*Limited Direct Execution*)
  - Ch. 7-9 (*Scheduling: FIFO, MLFQ, Proportional Share*)
- **Projects**:
  - Implement a **basic job scheduler** (FIFO, Round Robin).
  - Extend the shell with job control (`bg`, `fg`, `jobs`).
- **Time**: 12+ hours/week.

---

### **Weeks 5-6: Memory Management**
**Goals**: Implement memory allocation mechanisms.
- **Read**:
  - Ch. 14 (*Memory API*), Ch. 15 (*Address Translation*), Ch. 16 (*Segmentation*)
- **Projects**:
  - Write a **custom malloc/free implementation**.
  - Simulate a **simple page table with TLB**.
- **Time**: 12+ hours/week.

---

### **Weeks 7-8: Concurrency Basics**
**Goals**: Learn threading, synchronization, and race conditions.
- **Read**:
  - Ch. 26 (*Concurrency Introduction*), Ch. 27 (*Thread API*)
- **Projects**:
  - Implement a **multi-threaded web server**.
  - Implement **mutex locks & spinlocks**.
- **Time**: 12-15 hours/week.

---

### **Weeks 9-10: Advanced Concurrency**
**Goals**: Work on condition variables and semaphores.
- **Read**:
  - Ch. 28 (*Locks*), Ch. 29-31 (*Condition Variables, Semaphores*)
  - Ch. 32 (*Common Concurrency Problems*)
- **Projects**:
  - Implement the **bounded buffer (producer-consumer problem)**.
  - Implement a **thread-safe queue**.
- **Time**: 12+ hours/week.

---

### **Weeks 11-12: File Systems & I/O**
**Goals**: Learn about disk I/O and file systems.
- **Read**:
  - Ch. 36 (*I/O Devices*), Ch. 37 (*Hard Drives*), Ch. 38 (*RAID*)
  - Ch. 39 (*File & Directories API*), Ch. 40 (*File System Implementation*)
- **Projects**:
  - Implement a **basic file system on a virtual disk** (inode-based).
  - Write a **simple disk scheduling simulator**.
- **Time**: 12+ hours/week.

---

### **Weeks 13-14: Kernel Development (xv6) Begins**
**Goals**: Modify the xv6 kernel.
- **Read**:
  - Ch. 4 (*Process Abstraction*), Ch. 11 (*Summary of CPU Virtualization*)
  - xv6 process management reference.
- **Projects**:
  - Add a **system call** to xv6.
  - Implement **priority scheduling** in xv6.
- **Time**: 15+ hours/week.

---

### **Weeks 15-16: Virtual Memory in xv6**
**Goals**: Work with address spaces and paging.
- **Read**:
  - Ch. 20 (*Paging*), Ch. 21 (*Beyond Physical Memory*), Ch. 22 (*Page Replacement Policies*)
- **Projects**:
  - Implement **demand paging** in xv6.
  - Extend xv6 to **support swapping**.
- **Time**: 15+ hours/week.

---

### **Weeks 17-18: File System in xv6 & Crash Consistency**
**Goals**: Extend the xv6 file system.
- **Read**:
  - Ch. 42 (*FSCK & Journaling*), Ch. 43 (*Log-structured FS*), Ch. 44 (*Data Integrity*)
- **Projects**:
  - Implement **journaling in xv6 file system**.
  - Add **file permissions** to xv6.
- **Time**: 15+ hours/week.

---

### **Weeks 19-20: Distributed Systems (Minimal Effort)**
**Goals**: Cover distributed systems fundamentals.
- **Read**:
  - Ch. 46-49 (*Distributed Systems, NFS, AFS*) – **Skim key points only**.
- **Capstone Project** (choose one):
  - Implement a **user-mode file system** (FUSE-based).
  - Extend xv6 with **threading support**.
  - Develop an **advanced memory allocator**.
- **Time**: 15+ hours/week.

---

## **How to Track Progress**
1. **Use Git for tracking updates**
   ```sh
   git add study_plan.md
   git commit -m "Add OS self-study plan"

