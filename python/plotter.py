import re
import glob
import pandas as pd

import matplotlib.pyplot as plt
import seaborn as sns

sns.set(style="whitegrid")

def plot_pass_fail(df, save_path=None):
    """
    Plot pass/fail rate per RNG.
    """
    # Compute pass rate per RNG
    pass_rates = df.groupby('rng_name')['pass_fail'].mean().reset_index()
    pass_rates['pass_rate_percent'] = pass_rates['pass_fail'] * 100

    plt.figure(figsize=(10,6))
    sns.barplot(x='rng_name', y='pass_rate_percent', data=pass_rates, palette="viridis")
    plt.ylabel("Pass Rate (%)")
    plt.xlabel("RNG")
    plt.title("Pass Rate per RNG")
    plt.ylim(0, 100)

    if save_path:
        plt.savefig(save_path, bbox_inches='tight')
    else:
        plt.show()


def plot_pvalue_distribution(df, save_path=None):
    """
    Plot distribution of p-values per RNG.
    """
    plt.figure(figsize=(12,6))
    sns.histplot(data=df, x='p_value', hue='rng_name', bins=20, element="step", stat="probability", common_norm=False)
    plt.xlabel("p-value")
    plt.ylabel("Probability")
    plt.title("Distribution of p-values per RNG")
    plt.legend(title="RNG")

    if save_path:
        plt.savefig(save_path, bbox_inches='tight')
    else:
        plt.show()


def parse_testu01_file(file_path):
    """
    Parse a TestU01 log file into a DataFrame.
    Extracts test name, p-value(s), CPU time, pass/fail.
    """
    with open(file_path, 'r') as f:
        lines = f.readlines()

    results = []
    rng_name = None
    battery = None
    timestamp = None

    # Extract metadata
    for line in lines[:20]:  # first few lines
        if line.startswith("RNG,"):
            rng_name = line.strip().split(',')[1]
        elif line.startswith("Battery,"):
            battery = line.strip().split(',')[1]
        elif line.startswith("Timestamp,"):
            timestamp = line.strip().split(',')[1]

    test_name = None
    p_value = None
    notes = ""
    pass_fail = None

    for line in lines:
        # Test name line (usually ends with "test:")
        m = re.match(r"^\s*(\w+_\w+)\s+test:", line)
        if m:
            test_name = m.group(1)
            p_value = None
            notes = ""
            continue

        # p-value line
        m = re.search(r"p-value of test\s*:\s*([0-9.eE+-]+)", line)
        if m:
            p_value = float(m.group(1))
            # Decide pass/fail (p-value in [0,1] and not too extreme)
            if 0.0001 <= p_value <= 0.9999:
                pass_fail = 1
            else:
                pass_fail = 0
            results.append({
                "rng_name": rng_name,
                "battery": battery,
                "timestamp": timestamp,
                "test_name": test_name,
                "p_value": p_value,
                "pass_fail": pass_fail,
                "notes": notes
            })

    return pd.DataFrame(results)

def load_testu01_results(file_pattern="results/*.csv"):
    """
    Load multiple TestU01 result files into a single DataFrame
    """
    dfs = []
    for file in glob.glob(file_pattern):
        df = parse_testu01_file(file)
        dfs.append(df)
    if dfs:
        return pd.concat(dfs, ignore_index=True)
    else:
        return pd.DataFrame()
