import tkinter as tk
from tkinter import messagebox
from PIL import Image, ImageTk
import numpy as np
from math import exp

# -------------------------------
# Настройки
# -------------------------------

WIDTH = 32
HEIGHT = 32
SCALE = 4

BACKGROUND = 0.0
DRAW_VALUE = 1.0


class Painter:

    def __init__(self, root):

        self.root = root
        self.root.title("Grayscale Painter")
        self.root.resizable(False, False)

        self.image = np.full((HEIGHT, WIDTH), BACKGROUND, dtype=np.float64)

        # -----------------------------
        # Верхняя панель
        # -----------------------------

        top = tk.Frame(root, pady=5)
        top.pack(fill="x")

        tk.Label(top, text="Brush size").pack(side="left", padx=5)

        self.brush = tk.Scale(
            top,
            from_=2,
            to=5,
            orient=tk.HORIZONTAL,
            length=180
        )
        self.brush.set(3)
        self.brush.pack(side="left")

        # -----------------------------
        # Вторая панель
        # -----------------------------

        middle = tk.Frame(root, pady=5)
        middle.pack(fill="x")

        tk.Label(middle, text="Label").pack(side="left", padx=(5, 2))

        self.label_entry = tk.Entry(middle, width=6)
        self.label_entry.insert(0, "0")
        self.label_entry.pack(side="left")

        self.counter = 0

        self.counter_label = tk.Label(
            middle,
            text=f"Drawings: {self.counter}",
            font=("Arial", 10, "bold")
        )
        self.counter_label.pack(side="left", padx=20)

        # -----------------------------
        # Кнопки
        # -----------------------------

        buttons = tk.Frame(root, pady=5)
        buttons.pack(fill="x")

        tk.Button(
            buttons,
            text="Clear",
            width=12,
            command=self.clear
        ).pack(side="left", padx=5)

        tk.Button(
            buttons,
            text="Copy Array",
            width=15,
            command=self.copy_cpp
        ).pack(side="left", padx=5)

        tk.Button(
            buttons,
            text="Reset Counter",
            width=15,
            command=self.reset_counter
        ).pack(side="left", padx=5)

        # -----------------------------
        # Холст
        # -----------------------------

        self.canvas = tk.Canvas(
            root,
            width=WIDTH * SCALE,
            height=HEIGHT * SCALE,
            highlightthickness=1,
            highlightbackground="gray"
        )
        self.canvas.pack(padx=5, pady=5)

        self.tk_image = None

        self.canvas_image = self.canvas.create_image(
            0,
            0,
            anchor="nw"
        )

        self.refresh()

        self.canvas.bind("<Button-1>", self.paint)
        self.canvas.bind("<B1-Motion>", self.paint)

    # ----------------------------

    def refresh(self):

        img = (self.image * 255).astype(np.uint8)

        pil = Image.fromarray(img, mode="L")

        pil = pil.resize(
            (WIDTH * SCALE, HEIGHT * SCALE),
            Image.Resampling.NEAREST
        )

        self.tk_image = ImageTk.PhotoImage(pil)

        self.canvas.itemconfig(
            self.canvas_image,
            image=self.tk_image
        )

    # ----------------------------

    def paint(self, event):

        x0 = event.x / SCALE
        y0 = event.y / SCALE

        radius = self.brush.get() / 2
        sigma = radius * 0.45

        xmin = max(0, int(x0 - radius - 1))
        xmax = min(WIDTH, int(x0 + radius + 2))

        ymin = max(0, int(y0 - radius - 1))
        ymax = min(HEIGHT, int(y0 + radius + 2))

        for y in range(ymin, ymax):
            for x in range(xmin, xmax):

                dx = x - x0
                dy = y - y0

                d = (dx * dx + dy * dy) ** 0.5

                if d > radius:
                    continue

                alpha = exp(-(d * d) / (2 * sigma * sigma))

                self.image[y, x] = (
                    self.image[y, x] * (1 - alpha)
                    + DRAW_VALUE * alpha
                )

        self.refresh()

    # ----------------------------

    def clear(self):

        self.image.fill(BACKGROUND)
        self.refresh()

    # ----------------------------

    def update_counter(self):

        self.counter_label.config(
            text=f"Drawings: {self.counter}"
        )

    # ----------------------------

    def reset_counter(self):

        self.counter = 0
        self.update_counter()

    # ----------------------------

    def copy_cpp(self):

        try:
            label = int(self.label_entry.get())
        except ValueError:
            messagebox.showerror(
                "Error",
                "Label must be an integer."
            )
            return

        pixels = " ".join(
            f"{v:.6f}"
            for v in self.image.flatten()
        )

        # Формат:
        # label pixel1 pixel2 pixel3 ...

        text = f"{label}\n{pixels}"
        
        with open("file.txt", "a", encoding="utf-8") as file:
          file.write(text + "\n")
          
        

        self.root.clipboard_clear()
        self.root.clipboard_append("\n".join(
            f"{v:.6f}"
            for v in self.image.flatten()
        ))
        self.root.update()

        self.counter += 1
        self.update_counter()
        
        

        


# --------------------------------

root = tk.Tk()
Painter(root)
root.mainloop()

