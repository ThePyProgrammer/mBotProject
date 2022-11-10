import numpy as np
import pandas as pd
import matplotlib.pyplot as plt


for file in ["red_excitation.csv"]:
    df = pd.read_csv(file)#.rolling(10).mean()
    df = df[df["$t$"] <= 200]
    df.plot(x = "$t$", y = "LDR Reading")
    plt.show();