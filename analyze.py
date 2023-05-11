import numpy as np
import pandas as pd
import matplotlib.pyplot as plt 

ctlr_csv = pd.read_csv("ctlr.csv")
#plt.plot(ctlr_csv["x"])
#plt.plot(ctlr_csv["y"])
the_is = range(len(ctlr_csv["x"]))
    
plt.scatter(the_is, ctlr_csv["x"], s=0.5)
#x1, y1 = [min(ctlr_csv["x"]),min(ctlr_csv["y"])], [max(ctlr_csv["x"]),max(ctlr_csv["y"]),]
#print(min(ctlr_csv["x"]))
#print(max(ctlr_csv["x"]))
#plt.plot(x1, y1, marker = 'o')
plt.savefig("ctlr.png")