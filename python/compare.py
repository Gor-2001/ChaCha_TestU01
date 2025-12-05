import plotter

# Load all files matching pattern
df = plotter.load_testu01_results("results/chacha8*.csv")  # or any pattern
print(df.head())

# You can now feed this DataFrame to plotting functions
# Example: plot pass/fail rate per RNG
plotter.plot_pass_fail(df)
