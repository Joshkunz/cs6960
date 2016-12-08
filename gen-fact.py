import random

def sint_max(bit_depth):
    return 2**(bit_depth - 1) - 1

def fact(i):
    v = 1
    for i in range(1, i+1):
        v *= i
    return v

def compute_fact(i):
    if i == 1: return "1"
    return "(* {0} {1})".format(str(i), compute_fact(i - 1))

FACT_MAX = sint_max(bit_depth = 64) 
INDENT = "    "

preambles = []

prog = ""

i = 1
while True:
    factval = fact(i)
    indent = INDENT * (i - 1)
    if factval > FACT_MAX:
        preambles.append(("large", "# ARGS {0}\n# RESULT -1\n".format(random.randint(i, FACT_MAX), factval)))
        prog += "\n" + indent + "-1" + (")" * (i - 1))
        break
    name = "{0:0>2}".format(i)
    preambles.append((name, "# ARGS {0}\n# RESULT {1}\n".format(i, factval)))
    newline = "\n" if i > 1 else ""
    fact_line = compute_fact(i)
    prog += newline + indent + "(if (== a0 {0})\n{2}{1}".format(i, fact_line, indent + INDENT)
    i += 1

f_postfix = "_factorial.calc"

for (name, preamble) in preambles:
    with open(name + f_postfix, "w") as f:
        f.write(preamble + prog)
