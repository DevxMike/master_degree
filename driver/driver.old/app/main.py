from GUI import ControlGUI, tk

def main():
    broker = "mqtt-dashboard.com"
    port = 1883

    root = tk.Tk()
    gui = ControlGUI(root, broker, port)

    # Additional initialization of Cruise Control, Navigation, etc.
    root.after(10, gui.poll_events)
    root.mainloop()

if __name__ == "__main__":
    main()
