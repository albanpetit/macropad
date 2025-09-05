## Commit Convention

To keep a clear and structured history, commit messages must follow this convention:  

**type**(**scope**): **description**

### 1. Type

The type describes the nature of the change:  

- **feat** → New feature  
- **fix** → Bug fix  
- **style** → Formatting, indentation, renaming, with no impact on logic  
- **refactor** → Code or schematic refactoring without adding or fixing functionality  
- **perf** → Performance improvements  
- **test** → Adding or updating tests  
- **build** → Build system or dependency changes  
- **ci** → Continuous integration changes (workflow, actions, etc.)  
- **chore** → Other changes that do not modify the product itself  

### 2. Scope

The scope specifies the area affected by the commit:  

- **ecad** → Schematics, PCB, electronic components  
- **mcad** → 3D modeling, mechanics, enclosure  
- **fw** → Embedded firmware (low-level software)  
- **sw** → Software (applications, PC or web tools)  
- **doc** → User or developer documentation  
- **repo** → Repository structure, scripts, configuration  

### 3. Description

- Short and imperative (e.g., *"add status LED"* rather than *"added status LED"*).  
- Use either English or French, but stay consistent across the project.  

---

### Examples
feat(ecad): add USB-C connector to the schematic
fix(fw): fix key debounce issue
docs(repo): add commit convention to the README
refactor(sw): simplify macro management

---

👉 This convention makes it easy to **filter commits** by both change type and project area.  
