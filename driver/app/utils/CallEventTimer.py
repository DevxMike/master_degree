import time

class CallEventTimer:
    def __init__(self, interval_ms, callback):
        self.interval_ms = interval_ms
        self.callback = callback
        self._running = False
        self._start_time = 0

    def start(self):
        if not self._running:
            self._running = True
            self._start_time = int(time.time() * 1000)

    def stop(self):
        self._running = False

    def poll(self):
        if self._running:
            current_time = int(time.time() * 1000)
            if(current_time - self._start_time >= self.interval_ms):
                self.callback()
                self._start_time = current_time