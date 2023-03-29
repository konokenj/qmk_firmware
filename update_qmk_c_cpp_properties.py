import argparse
import json
import os
import shlex
import sys
import time

C_CPP_PROPERTIES_PATH = "./.vscode/c_cpp_properties.json"

def watch(action, path: str):
    print("error: watchdog is not installed.", file=sys.stderr)
    print("If you use the --watch option, you should install watchdog.", file=sys.stderr)
    print("> pip install watchdog", file=sys.stderr)
    exit(1)


try:
    from watchdog.events import FileSystemEventHandler
    from watchdog.observers import Observer


    class ChangeHandler(FileSystemEventHandler):
        def __init__(self, action):
            super().__init__()
            self.action = action


        def on_modified(self, event):
            self.action()
            print("update .vscode/c_cpp_properties.json")

    def watch(action, path: str):
        handler = ChangeHandler(action)
        observer = Observer()
        observer.schedule(handler, os.path.dirname(path))
        observer.start()
        print("watching...")
        try:
            while True:
                time.sleep(1)
        except KeyboardInterrupt:
            observer.stop()
        observer.join()
except ImportError:
    pass


def get_cflags_path(keyboard: str, keymap: str):
    return f"./.build/obj_{keyboard}_{keymap}/cflags.txt"

def parse_cflags(path: str):
    with open(path) as f:
        cflags = f.read()

    parser = argparse.ArgumentParser()
    parser.add_argument('-D', action="append")
    parser.add_argument('-I', action="append")
    parser.add_argument('-include', action="append")
    args, _ = parser.parse_known_args(shlex.split(cflags))
    return args

def main(keyboard: str, keymap: str):
    args = parse_cflags(get_cflags_path(keyboard, keymap))

    with open(C_CPP_PROPERTIES_PATH, "r") as f:
        props = json.load(f)

    for conf in props['configurations']:
        conf["defines"] = args.D
        conf["includePath"] = ["${workspaceFolder}/" + arg for arg in args.I]
        conf["forcedInclude"] = ["${workspaceFolder}/" + arg for arg in args.include]

    with open(C_CPP_PROPERTIES_PATH, "w") as f:
        json.dump(props, f, indent=4, ensure_ascii=False)

if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument("-kb", "--keyboard", required=True)
    parser.add_argument("-km", "--keymap", required=True)
    parser.add_argument("--watch", action="store_true", default=False)
    args = parser.parse_args()

    action = lambda: main(args.keyboard, args.keymap)

    action()
    if args.watch:
        watch(action, get_cflags_path(args.keyboard, args.keymap))
