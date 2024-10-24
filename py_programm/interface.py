import tkinter as tk
from tkinter import ttk, messagebox
import ctypes
import numpy as np
from PIL import Image, ImageTk

class PointCharge(ctypes.Structure):
    _fields_ = [("x", ctypes.c_int),
                ("y", ctypes.c_int),
                ("value", ctypes.c_double)]

class InterfaceApp:
    def __init__(self, mainroot):
        self.root = mainroot
        self.root.title("Интерфейс программы")
        self.root.state('normal')
        self.root.resizable(True, True)
        self.root.configure(bg="#3b3b3b")

        self.canvas = tk.Canvas(mainroot, bg="#707070", highlightthickness=0)

        self.rows = 850
        self.cols = 850
        self.canvas.place(relx=0, rely=0, width=self.cols, height=self.rows)

        self.info_near_cursor = tk.Label(mainroot, bg="white")
        self.info_near_cursor.place(x=100000, y=100000)

        self.canvas.bind('<Motion>', self.show_info)
        self.canvas.bind('<Leave>', self.hide_coord_label)
        self.canvas.bind('<Button-1>', self.open_input_window)

        self.create_menu()

        self.current_mode = "Ввод"
        self.points = []
        self.entries = []
        self.buttons = []
        self.background_image = None
        self.canvas_image_id = None
        self.electric_field = None
        self.potential_field = None
        self.point_label = None
        self.create_right_panel()

    def create_menu(self):
        menubar = tk.Menu(self.root, bg="#707070")

        mode_menu = tk.Menu(menubar, tearoff=0)
        mode_menu.add_command(label="Ввод", command=self.on_input_button)
        mode_menu.add_command(label="Потенциалы", command=self.on_potentials_button)
        mode_menu.add_command(label="Электрическая напряженность",
                              command=self.on_electric_field_button)
        menubar.add_cascade(label="Режим", menu=mode_menu)

        self.root.config(menu=menubar)

    def create_right_panel(self):
        self.create_entry_with_placeholder("Введите координату по вертикали", 0.2)
        self.create_entry_with_placeholder("Введите координату по горизонтали", 0.27)
        self.create_entry_with_placeholder("Введите значения заряда", 0.34)

        button1 = ttk.Button(self.root, text="Добавить введенный заряд", command=self.add_charge)
        button1.place(relx=0.65, rely=0.41, relwidth=0.3, relheight=0.05)

        button3 = ttk.Button(self.root, text="Очистить", command=self.clear_points)
        button3.place(relx=0.65, rely=0.77, relwidth=0.3, relheight=0.05)

        self.buttons += [button1, button3]

    def create_entry_with_placeholder(self, placeholder, y):
        entry = ttk.Entry(self.root)
        entry.place(relx=0.65, rely=y, relwidth=0.3, relheight=0.05)

        entry.config(foreground='grey')
        entry.insert(0, placeholder)
        entry.bind("<FocusIn>", lambda e: self.on_entry_click(e, placeholder))
        entry.bind("<FocusOut>", lambda e: self.on_focusout(e, placeholder))

        entry.bind("<KeyPress>", lambda e: self.remove_placeholder_on_keypress(e, entry, placeholder))

        self.entries.append(entry)

    def clear_points(self):
        for point in self.points:
            self.canvas.delete(point[0])
        self.points.clear()

    def on_entry_click(self, event, placeholder):
        entry = event.widget
        if entry.get() == placeholder:
            entry.delete(0, tk.END)
            entry.config(foreground="black")

    def on_focusout(self, event, placeholder):
        entry = event.widget
        if entry.get() == "":
            entry.insert(0, placeholder)
            entry.config(foreground="gray")

    def remove_placeholder_on_keypress(self, event, entry, placeholder):
        if entry.get() == placeholder:
            entry.delete(0, tk.END)

    def show_info(self, event):
        try:
            self.info_near_cursor.place_forget()
        except Exception:
            pass

        x, y = event.x, event.y
        if self.current_mode == "Потенциалы":
            val = self.potential_field[y][x]
            self.info_near_cursor.config(text=f"потенциал: {val}")
        elif self.current_mode == "Ввод":
            self.info_near_cursor.config(text=f"x: {y}, y: {x}")

        self.info_near_cursor.place(x=x + 10, y=y - 10)

    def create_point(self, x, y, float_value):
        current_point = PointCharge()
        current_point.x = y
        current_point.y = x
        current_point.value = float_value

        radius = 5
        x1 = x - radius
        y1 = y - radius
        x2 = x + radius
        y2 = y + radius
        point = self.canvas.create_oval(x1, y1, x2, y2, fill="yellow", outline="yellow")

        self.canvas.tag_bind(point, '<Enter>', lambda event: self.show_point_label(event, x, y, float_value))
        self.canvas.tag_bind(point, '<Leave>', self.hide_point_label)
        self.canvas.tag_bind(point, '<Button-3>', lambda event: self.delete_point(point))
        self.points.append((point, current_point))

    def hide_coord_label(self, event):
        self.info_near_cursor.place(x=event.x + 10000, y=event.y + 10000)

    def open_input_window(self, event):
        if self.current_mode != "Ввод":
            return
        input_window = tk.Toplevel(self.root)
        input_window.title("Введите заряд")
        input_window.resizable(False, False)
        input_window.geometry("150x120")

        input_window.after(100, input_window.grab_set)

        tk.Label(input_window, text="Введите заряд (нк):").pack(pady=10)

        entry = tk.Entry(input_window)
        entry.pack(pady=5)

        submit_button = tk.Button(input_window, text="Подтвердить",
                                  command=lambda: self.submit_value(entry.get(), input_window, event))
        submit_button.pack(pady=10)

    def submit_value(self, value, window, event):
        try:
            float_value = float(value)
            if float_value == 0:
                messagebox.showerror("Ошибка", "Пожалуйста, введите корректное дробное значение.")
                return
            self.create_point(event.x, event.y, float_value)
            window.destroy()
        except ValueError:
            messagebox.showerror("Ошибка", "Пожалуйста, введите корректное дробное значение.")

    def show_point_label(self, event, x, y, label):
        if self.point_label is None:
            self.point_label = tk.Label(self.root, text=str(label), bg="white", borderwidth=1, relief="solid")
            self.point_label.place(x=x + 10, y=y - 20)

    def hide_point_label(self, event):
        if self.point_label is not None:
            self.point_label.place_forget()
            self.point_label = None

    def delete_point(self, point):
        if self.current_mode == "Ввод":
            self.canvas.delete(point)
            self.rm_from_list_point(point)
            self.hide_point_label(None)

    def rm_from_list_point(self, seeking):
        for i in self.points:
            if i[0] == seeking:
                self.points.remove(i)

    def hide_inputs_and_buttons(self):
        for entry in self.entries:
            entry.place_forget()
        self.entries.clear()
        for button in self.buttons:
            button.place_forget()
        self.buttons.clear()
        self.info_near_cursor.place_forget()

        if hasattr(self, 'canvas_image_id'):
            self.canvas.delete(self.canvas_image_id)
            del self.canvas_image_id

    def interpolate_color(self, color1, color2, factor):
        return tuple([
            int(color1[i] + (color2[i] - color1[i]) * factor) for i in range(3)
        ])

    def get_color(self, percentile):
        blue = (0, 255, 255)
        yellow = (255, 255, 0)
        scarlet = (255, 36, 0)

        lp = 5
        d = 50 - lp
        mp = lp + d
        up = mp + d

        if percentile <= lp:
            return blue
        elif lp < percentile <= mp:
            factor = (percentile - lp) / d
            return self.interpolate_color(blue, yellow, factor)
        elif mp < percentile <= up:
            factor = (percentile - mp) / d
            return self.interpolate_color(yellow, scarlet, factor)
        else:
            return scarlet

    def normalize_matrix(self, mtrx):
        matrix = np.array(mtrx)

        flat_matrix = matrix.flatten()
        sorted_flat_matrix = np.sort(flat_matrix)
        percentiles = np.percentile(sorted_flat_matrix, np.arange(0, 100, 1))
        percentile_matrix = np.searchsorted(percentiles, matrix, side='right')

        return percentile_matrix

    def rgb_to_hex(self, rgb):
        return '#{:02x}{:02x}{:02x}'.format(*rgb)

    def draw_potential_field(self):
        image = Image.new('RGB', (self.cols, self.rows))
        pixels = image.load()
        tmp = self.normalize_matrix(self.potential_field)

        for row in range(self.rows):
            for col in range(self.cols):
                value = tmp[row][col]
                color = self.get_color(value)
                pixels[col, row] = color

        image.save("img.png", format="PNG")

        image = image.resize((self.cols, self.rows))

        self.background_image = ImageTk.PhotoImage(image)

        if hasattr(self, 'canvas_image_id'):
            self.canvas.itemconfig(self.canvas_image_id, image=self.background_image)
        else:
            self.canvas_image_id = self.canvas.create_image(0, 0, anchor="nw", image=self.background_image)
            self.canvas.tag_lower(self.canvas_image_id)

    def on_input_button(self):
        self.hide_inputs_and_buttons()
        self.create_right_panel()
        self.current_mode = "Ввод"
        self.potential_field = None
        self.electric_field = None

    def call_lol_kek_cheburek_wrapper(self, rows, cols, charges):
        import os
        if os.name == "posix":
            lib = ctypes.CDLL('./for_py_potential/libpotential_field.so')
        else:
            lib = ctypes.CDLL('./for_py_potential/libpotential_field.dll')
        
        lib.lol_kek_cheburek_wrapper.argtypes = [ctypes.c_int, ctypes.c_int, ctypes.POINTER(PointCharge), ctypes.c_int]
        lib.lol_kek_cheburek_wrapper.restype = ctypes.POINTER(ctypes.POINTER(ctypes.c_double))

        charge_array = (PointCharge * len(charges))(*charges)

        result = lib.lol_kek_cheburek_wrapper(rows, cols, charge_array, len(charges))

        matrix = np.zeros((rows, cols), dtype=np.float64)
        for i in range(rows):
            for j in range(cols):
                matrix[i, j] = result[i][j]

        return matrix.tolist()

    def on_potentials_button(self):
        self.hide_inputs_and_buttons()
        self.current_mode = "Потенциалы"
        charges = [i[1] for i in self.points]
        self.potential_field = self.call_lol_kek_cheburek_wrapper(self.rows, self.cols, charges)
        self.draw_potential_field()

    def on_electric_field_button(self):
        self.hide_inputs_and_buttons()

    def calculate_potential(self, x, y):
        pass

    def add_charge(self):
        try:
            y = float(self.entries[0].get())
            x = float(self.entries[1].get())
            charge = float(self.entries[2].get())

            if y < 0 or y >= self.rows or x < 0 or x >= self.cols:
                messagebox.showerror("Ошибка", "Координаты должны быть в пределах от 0 до {}.".format(self.rows - 1))
                return

            if charge == 0:
                messagebox.showerror("Ошибка", "Заряд не может быть равен 0.")
                return

            self.create_point(int(x), int(y), charge)

            self.entries[0].delete(0, tk.END)
            self.entries[1].delete(0, tk.END)
            self.entries[2].delete(0, tk.END)

            self.entries[0].focus_set()

        except ValueError:
            messagebox.showerror("Ошибка", "Пожалуйста, введите корректные числовые значения.")

if __name__ == "__main__":
    root = tk.Tk()
    app = InterfaceApp(root)
    root.mainloop()
