"""Exercise the graph executable and API using isolated temporary files."""
from pathlib import Path
import subprocess
import tempfile

ROOT = Path(__file__).resolve().parents[1]
EXE = ROOT / "graph/antenna-graph"

with tempfile.TemporaryDirectory() as directory:
    result = subprocess.run([str(ROOT / "graph/graph-tests")], cwd=directory,
                            capture_output=True, text=True, check=True)
    visits = [line for line in result.stdout.splitlines() if line.startswith("BFS visit:")]
    assert len(visits) == 4 and len(set(visits)) == 4, result.stdout
    print(result.stdout.splitlines()[-1])


def run_map(content):
    with tempfile.TemporaryDirectory() as directory:
        if content is not None:
            Path(directory, "matrix.txt").write_bytes(content)
        return subprocess.run([str(EXE)], cwd=directory, capture_output=True, text=True)

result = run_map((ROOT / "graph/matrix.txt").read_bytes())
assert result.returncode == 0
assert result.stdout.count("Frequency:") == 4
assert "Intersection at (5.33,4.86)" in result.stdout
result = run_map(b'.A.A...')
assert result.returncode == 0 and "Interference at (0, 5)" in result.stdout
result = run_map(b'.A.A...\r\n')
assert result.returncode == 0 and result.stdout.count("Frequency:") == 2
assert "Interference at (0, 5)" in result.stdout
for content in (None, b'', b'......\n', b'\n', b'A' * 254 + b'\n'):
    assert run_map(content).returncode == 1
print('PASS: graph demo, CRLF, final line, invalid/empty/missing input')
