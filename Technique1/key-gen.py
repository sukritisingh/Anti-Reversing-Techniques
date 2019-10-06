import os
import subprocess
# import Popen, PIPE, STDOUT/......
# password = 'p42KPIF0T1uxKspPewAxOL2mI5R5Dtdt'
#os.system("""echo rSrV54jE3gWz8lcocuV2f66XAFAbNyIO '\|' nc 172.16.218.1 1270""")
#cmd = 'echo rSrV54jE3gWz8lcocuV2f66XAFAbNyIO | nc 172.16.218.1 1270'
# cmd1 = 'VOwh42NBmFLRnfRi0SEh0eDoVpv6lD0W'
# #subprocess.run(cmd) # returns the exit status
# p = Popen(['sh'], stdout=PIPE, stdin=PIPE, stderr=STDOUT)    
# grep_stdout = p.communicate(input=r'date')[0]
# print(grep_stdout.decode())
# import pty; pty.spawn("/bin/bash")

os.system("nc 172.16.218.1 1270  >>  VOwh42NBmFLRnfRi0SEh0eDoVpv6lD0W ")
