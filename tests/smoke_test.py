"""Run executable-level regression checks using temporary input directories."""
from pathlib import Path
import subprocess
import tempfile

ROOT = Path(__file__).resolve().parents[1]
EXE = ROOT / "antenna-analyzer"

def run_map(text):
    with tempfile.TemporaryDirectory() as folder:
        Path(folder, "antennas.txt").write_text(text)
        result = subprocess.run([str(EXE)], cwd=folder, capture_output=True, text=True)
        return result, Path(folder, "antennas.bin").exists()

def grid(output):
    return [line for line in output.splitlines() if line and set(line) <= set('.#A0B')]

result, saved = run_map((ROOT / "antennas.txt").read_text())
assert result.returncode == 0 and saved
assert "9 rows x 12 columns" in result.stdout
assert result.stdout.count("Frequency:") == 6
assert grid(result.stdout) == [
    '......#....#', '...#....0...', '....#0....#.', '..#....0....',
    '....0....#..', '.#....A.....', '...#........', '#...........', '........A...',
]
result, saved = run_map('.A.A...\n')
assert result.returncode == 0 and saved
assert grid(result.stdout) == ['.A.A.#.']
result, _ = run_map('.A.B...\n')
assert result.returncode == 0
assert grid(result.stdout) == ['.A.B...']
result, saved = run_map('.......\n')
assert result.returncode == 1 and not saved
print('PASS: sample grid, equal frequencies, different frequencies, empty map')
