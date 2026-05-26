
import tkinter as tk
from tkinter import ttk, scrolledtext, filedialog, messagebox
import threading
import subprocess
import queue
import os

BASE_DIR = r"C:\Users\HP\OneDrive\Desktop\C-Lite"

COMMANDS = {
    "P1": [
        "flex lexer.l",
        "gcc lex.yy.c -o a.exe",
        "a.exe < test.c"
    ],
    "P2": [
        "bison -d parser.y",
        "flex lexl.l",
        "gcc lex.yy.c parser.tab.c symboltable.c -o a.exe",
        "a.exe < test.c"
    ],
    "P3": [
        "flex lexer.l",
        "bison -d parser.y",
        "gcc parser.tab.c lex.yy.c symboltable.c -o a.exe",
        "a.exe < test.c"
    ],
    "P4": [
        "bison -d parser.y",
        "flex lexer.l",
        "gcc lex.yy.c parser.tab.c symboltable.c -o a.exe",
        "a.exe < test.c"
    ]
}

class CommandRunner:
    def __init__(self, output_queue, stop_flag):
        self.output_queue = output_queue
        self.stop_flag = stop_flag

    def run_commands(self, folder):
        folder_path = os.path.join(BASE_DIR, folder)
        exe_path = os.path.join(folder_path, "a.exe")
        try:
            if os.path.exists(exe_path):
                os.remove(exe_path)
        except PermissionError:
            self.output_queue.put("❌ Cannot delete a.exe — it is running. Please close it manually.")
            return

        if not os.path.isdir(folder_path):
            self.output_queue.put(f"\n❌ ERROR: Folder '{folder_path}' does not exist.\n")
            return

        cmds = COMMANDS.get(folder)
        if not cmds:
            self.output_queue.put(f"\n✨ No commands defined for folder '{folder}'.\n")
            return

        for cmd in cmds:
            if self.stop_flag.is_set():
                self.output_queue.put("\n⏸ Execution stopped by user.\n")
                return

            self.output_queue.put(f"\n> {cmd}\n")

            try:
                proc = subprocess.Popen(cmd, shell=True, cwd=folder_path,
                                        stdout=subprocess.PIPE,
                                        stderr=subprocess.PIPE,
                                        text=True)
                stdout, stderr = proc.communicate()
                self.output_queue.put(stdout + stderr)

            except Exception as e:
                self.output_queue.put(f"❌ Error running command '{cmd}': {e}\n")
                return

        self.output_queue.put("\n✅ All commands executed.\n")

class CombinedApp(tk.Tk):
    def __init__(self):
        super().__init__()
        self.title("🌸 C-Lite Project & Editor 🌸")
        self.geometry("950x700")
        self.configure(bg="#FFF0F6")

        self.output_queue = queue.Queue()
        self.stop_flag = threading.Event()
        self.thread = None

        self.style = ttk.Style(self)
        self.style.theme_use('clam')
        self.setup_styles()

        self.notebook = ttk.Notebook(self)
        self.notebook.pack(fill=tk.BOTH, expand=True)

        self.create_editor_tab()
        self.create_project_tab()

        self.after(100, self.process_output_queue)

    def setup_styles(self):
        self.style.configure('TButton', font=('Comic Sans MS', 11, 'bold'), padding=6,
                             foreground='#4B0082', background='#E6C9F2')
        self.style.map('TButton', background=[('active', '#D88CEF')], foreground=[('active', 'white')])
        self.style.configure('TCombobox', padding=6, font=('Comic Sans MS', 11, 'bold'),
                             foreground='#6A0DAD', background='white')

    def create_editor_tab(self):
        self.editor_frame = tk.Frame(self.notebook, bg="#FDF3FF")
        self.notebook.add(self.editor_frame, text="📝 Code Editor")

        tk.Label(self.editor_frame, text="Write your C code below:",
                 font=("Comic Sans MS", 14, "bold"), bg="#FDF3FF").pack(pady=10)

        self.code_input = scrolledtext.ScrolledText(self.editor_frame, height=20, font=("Consolas", 12))
        self.code_input.pack(fill=tk.BOTH, padx=10, pady=5, expand=True)

        tk.Label(self.editor_frame, text="Select Phase to Run:",
                 font=("Comic Sans MS", 12, "bold"), bg="#FDF3FF").pack(pady=5)

        self.editor_phase_selector = ttk.Combobox(self.editor_frame, values=["P1", "P2", "P3", "P4"], state="readonly")
        self.editor_phase_selector.pack(pady=5)

        btn_frame = tk.Frame(self.editor_frame, bg="#FDF3FF")
        btn_frame.pack(pady=10)

        ttk.Button(btn_frame, text="💾 Save", command=self.save_file).pack(side=tk.LEFT, padx=5)
        ttk.Button(btn_frame, text="📂 Open", command=self.open_file).pack(side=tk.LEFT, padx=5)
        ttk.Button(btn_frame, text="▶ Compile & Run", command=self.run_code).pack(side=tk.LEFT, padx=5)
        ttk.Button(btn_frame, text="🧹 Clear Output", command=self.clear_output).pack(side=tk.LEFT, padx=5)

        self.output_text = scrolledtext.ScrolledText(self.editor_frame, height=10, font=("Consolas", 12),
                                                     bg="#FDEEF9", fg="#4B0082")
        self.output_text.pack(fill=tk.BOTH, padx=10, pady=(0, 10), expand=True)

    def run_code(self):
        code = self.code_input.get("1.0", tk.END).strip()
        if not code:
            messagebox.showwarning("Empty", "Code editor is empty!")
            return

        selected_phase = self.editor_phase_selector.get()
        if not selected_phase:
            messagebox.showwarning("Phase not selected", "Please select a compiler phase from the dropdown.")
            return

        self.output_text.delete("1.0", tk.END)

        test_file_path = os.path.join(BASE_DIR, "test.c")
        try:
            with open(test_file_path, 'w') as f:
                f.write(code)
        except Exception as e:
            self.output_text.insert(tk.END, f"❌ Could not write test.c: {e}\n")
            return

        phase_path = os.path.join(BASE_DIR, selected_phase)
        exe_path = os.path.join(phase_path, "a.exe")

        try:
            if os.path.exists(exe_path):
                os.remove(exe_path)
        except PermissionError:
            self.output_text.insert(tk.END, f"❌ Cannot delete a.exe in {selected_phase} — please close it manually.\n")
            return

        self.output_text.insert(tk.END, f"================ {selected_phase}: COMPILATION & EXECUTION ================\n")

        phase_cmds = COMMANDS.get(selected_phase, [])
        for cmd in phase_cmds:
            proc = subprocess.run(cmd, shell=True, cwd=phase_path,
                                  stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
            output = proc.stdout + proc.stderr
            self.output_text.insert(tk.END, f"> {cmd}\n{output}\n")

    def save_file(self):
        file_path = filedialog.asksaveasfilename(defaultextension=".c",
                                                 filetypes=[("C Files", "*.c"), ("All Files", "*.*")])
        if file_path:
            try:
                with open(file_path, 'w') as f:
                    f.write(self.code_input.get("1.0", tk.END))
                messagebox.showinfo("Success", "File saved successfully!")
            except Exception as e:
                messagebox.showerror("Error", f"Failed to save file: {e}")

    def open_file(self):
        file_path = filedialog.askopenfilename(filetypes=[("C Files", "*.c"), ("All Files", "*.*")])
        if file_path:
            try:
                with open(file_path, 'r') as f:
                    content = f.read()
                    self.code_input.delete("1.0", tk.END)
                    self.code_input.insert(tk.END, content)
            except Exception as e:
                messagebox.showerror("Error", f"Failed to open file: {e}")

    def clear_output(self):
        self.output_text.delete("1.0", tk.END)

    def create_project_tab(self):
        self.project_frame = tk.Frame(self.notebook, bg="#FFF3FC")
        self.notebook.add(self.project_frame, text="🛠 Projects")

        tk.Label(self.project_frame, text="Select Project Folder:", font=("Comic Sans MS", 14, "bold"),
                 bg="#FFF3FC").pack(pady=10)

        self.project_selector = ttk.Combobox(self.project_frame, values=list(COMMANDS.keys()), state="readonly")
        self.project_selector.pack(pady=5)

        btn_frame = tk.Frame(self.project_frame, bg="#FFF3FC")
        btn_frame.pack(pady=10)

        ttk.Button(btn_frame, text="▶ Run", command=self.start_execution).pack(side=tk.LEFT, padx=5)
        ttk.Button(btn_frame, text="⏹ Stop", command=self.stop_execution).pack(side=tk.LEFT, padx=5)

        self.project_output = scrolledtext.ScrolledText(self.project_frame, height=20, font=("Consolas", 12),
                                                        bg="#F0E6FF", fg="#4B0082")
        self.project_output.pack(fill=tk.BOTH, padx=10, pady=10, expand=True)

    def start_execution(self):
        folder = self.project_selector.get()
        if self.thread and self.thread.is_alive():
            messagebox.showwarning("Busy", "A process is already running!")
            return

        self.stop_flag.clear()
        self.thread = threading.Thread(target=CommandRunner(self.output_queue, self.stop_flag).run_commands,
                                       args=(folder,), daemon=True)
        self.thread.start()

    def stop_execution(self):
        self.stop_flag.set()

    def process_output_queue(self):
        while not self.output_queue.empty():
            output = self.output_queue.get_nowait()
            self.project_output.insert(tk.END, output)
            self.project_output.see(tk.END)
        self.after(100, self.process_output_queue)

if __name__ == "__main__":
    app = CombinedApp()
    app.mainloop()
