# 🏦 Banking Transaction System with OS Concepts

A **C-based banking simulation** that integrates core Operating System concepts — **Round Robin CPU Scheduling**, **Memory Management (Paging)**, and **Thread Synchronization (pthreads)** — into a real-world banking transaction model.

---

## 📌 Features

- ✅ Create bank accounts with initial balances
- ✅ Deposit and withdraw funds with mutex-protected concurrency
- ✅ Memory allocation using a **paging system** (page size: 10 units)
- ✅ CPU scheduling via **Round Robin algorithm** (time quantum: 3 units)
- ✅ Transaction history logging
- ✅ Memory heap visualization
- ✅ Gantt chart simulation of transaction execution

---

## 🧠 OS Concepts Demonstrated

| Concept | Implementation |
|---|---|
| **CPU Scheduling** | Round Robin with configurable time quantum |
| **Memory Management** | Fixed-size paging with dynamic allocation/deallocation |
| **Process Synchronization** | POSIX `pthread_mutex_t` for thread-safe operations |
| **Concurrency** | Per-account mutexes to prevent race conditions |

---

## 📁 Project Structure

```
banking-os-simulation/
│
├── banking.c        # Main source file (all logic included)
└── README.md        # Project documentation
```

---

## ⚙️ Configuration

You can modify these constants at the top of `banking.c`:

```c
#define MEMORY_SIZE      100   // Total memory units
#define PAGE_SIZE        10    // Size of each memory page
#define TIME_QUANTUM     3     // Round Robin time quantum
#define MAX_TRANSACTIONS 100   // Max transactions allowed
#define MAX_ACCOUNTS     50    // Max accounts allowed
```

---

## 🚀 Getting Started

### Prerequisites

- GCC compiler
- POSIX thread support (`libpthread`)
- Linux / macOS / WSL on Windows

### 🐧 Linux / macOS / WSL (Recommended)

```bash
# Clone the repository
git clone https://github.com/your-username/banking-os-simulation.git
cd banking-os-simulation

# Compile
gcc -o banking banking.c -lpthread

# Run
./banking
```

### 🪟 Windows (Native)

> ⚠️ `pthread` is a POSIX library and is **not natively available on Windows**.  
> It is **strongly recommended** to use **WSL (Windows Subsystem for Linux)** instead.

If you must use native Windows, install [pthreads-win32](https://sourceware.org/pthreads-win32/) and link it manually during compilation.

---

## 🖥️ Usage

Once running, you'll see an interactive menu:

```
Menu:
1. Create Account
2. Deposit Money
3. Withdraw Money
4. Show Transaction History and Reports
0. Exit
```

### Example Session

```
Enter your choice: 1
Enter Account ID: 101
Enter Initial Balance: 5000
Account 101 created with balance 5000.00

Enter your choice: 2
Enter Account ID: 101
Enter Deposit Amount: 1500
Deposited 1500.00 to account 101

--- Round Robin Scheduling ---
Transaction 0 executes from 0 to 3
Transaction 0 executes from 3 to 5
Total time: 5 units
```

---

## 📊 Reports & Visualizations

Select option **4** from the menu to view:

- **Memory Heap** — shows which memory pages are occupied by which transactions
- **Gantt Chart** — shows remaining durations of all logged transactions

```
--- Transaction History and Reports ---

Memory Heap:
Page 0 -> Transaction 0
Page 1 -> Empty
...

Gantt Chart:
Transaction 0: Duration 0
Transaction 1: Duration 0
```

---

## 🔒 Thread Safety

- A **global mutex** (`account_mutex`) protects the account list during creation and lookup.
- Each account has its own **individual mutex** (`accounts[i].mutex`) for balance modifications, enabling fine-grained locking and maximizing concurrency.
- A **memory mutex** (`memory_mutex`) guards the shared memory page map.

---

## ⚠️ Known Limitations

- The program is single-threaded in execution — pthreads are initialized but transactions run sequentially for simulation purposes.
- `rand()` is used for transaction duration, so Gantt chart results vary each run.
- No persistent storage — all data is lost on exit.
- Account IDs are not validated for duplicates.

---

## 🛠️ Possible Improvements

- [ ] Add file-based persistence (save/load accounts)
- [ ] Implement actual multi-threaded transaction processing
- [ ] Add transfer between accounts
- [ ] Validate duplicate account IDs
- [ ] Add a graphical/web-based frontend

---

## 📚 Academic Context

This project is designed to demonstrate the integration of the following OS topics:

- **Chapter: CPU Scheduling** → Round Robin Scheduler
- **Chapter: Memory Management** → Paging and page allocation
- **Chapter: Synchronization** → Mutex locks and critical sections



This project is open source and available under the [MIT License](LICENSE).
