# CacheMemorySim

<details>
  <summary>Table of Contents</summary>
  <ol>
    <li>
      <a href="#about-the-project">About The Project</a>
    </li>
    <li>
      <a href="#getting-started">Getting Started</a>
    </li>
    <li>
	<a href="#usage">Usage</a>
    	<ul>
		<li><a href="#input-format">Input Format</a></li>
		<li><a href="#clean-project">Clean Project</a></li>
		<li><a href="#example">Example</a></li>
	</ul>
    </li>
    <li><a href="#license">License</a></li>
    <li><a href="#acknowledgments">Acknowledgments</a></li>
  </ol>
</details>

# About The Project
This is a C++ container that runs a Cache and Memory Hierarchy Simulator.

# Getting Started

To get started you must have `g++` installed to run this program.

To install the G++ compiler on Linux run:

```bash
sudo apt install g++
```

Verify that you have `g++`:

```
g++ --version
```

# Usage

To compile the program run:

```bash
make
```

## Input Format

After running `make`, to run the executable enter this command with the following arguments:
```bash
./sim_cache <BLOCKSIZE> <L1_SIZE> <L1_ASSOC> <L2_SIZE> <L2_ASSOC> <REPLACEMENT_POLICY> <INCLUSION_PROPERTY> <trace_file>
```

### Input Arguments
- **BLOCKSIZE**: 
	- `unsigned int`. 
	- Block size in bytes. (Same block size for all caches in the memory hierarchy.)
- **L1_SIZE**: 
	- `unsingned int`. 
	- L1 cache size in bytes.
- **L1_ASSOC**: 
	- `unsigned int`. 
	- L1 set-associativity (1 is direct-mapped)
- **L2_SIZE**: 
	- `unsigned int`. 
	- L2 cache size in bytes.
- **L2_ASSOC**`: 
	- `unsigned int`. 
	- L2 set-associativity (1 is direct-mapped)
- **REPLACEMENT_POLICY**: 
	- `unsigned int`.
	- 0 for LRU
	- 1 for PLRU
	- 2 for Optimal
- **INCLUSION_PROPERTY**: 
	- `unsigned int`.
	- 0 for non-inclusive
	- 1 for inclusive
- **trace_file**: 
	- `std::string`.
	- Full name of trace file including any extensions.

## Clean Project

After running `make` and `./sim_cache`, clean the project:

```bash
make clean
```

To force clean, run:

```bash
make clobber
```

## Example 

Here is an example:

```bash
./sim_cache 32 8192 4 262144 8 0 0 gcc_trace.txt
```

**BLOCKSIZE** = 32 bytes

**L1 Cache**:

- 8192 Bytes ~ 8KB
- 4-way set-associative

**L2 Cache**:

- 262144 Bytes ~ 262KB
- 8-way set-associative

**REPLACEMENT_POLICY** = 0 = LRU

**INCLUSION_POLICY** = 0 = non-inclusive cache (default)

**trace_file** = `gcc_trace.txt`

# License

Distributed under the MIT License. See `LICENSE` for more information.

# Acknowledgments

- [The Pitchfork Layout](https://api.csswg.org/bikeshed/?force=1&url=https://raw.githubusercontent.com/vector-of-bool/pitchfork/develop/data/spec.bs)
