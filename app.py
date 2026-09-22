import asyncio
import os
import subprocess
import threading
from queue import Queue, Empty
from fastapi import FastAPI, WebSocket, WebSocketDisconnect
from fastapi.middleware.cors import CORSMiddleware

app = FastAPI()

app.add_middleware(
    CORSMiddleware,
    allow_origins=["*"],
    allow_credentials=True,
    allow_methods=["*"],
    allow_headers=["*"],
)

@app.websocket("/ws/session")
async def websocket_session(websocket: WebSocket):
    await websocket.accept()

    exe_path = os.path.abspath("main.exe")
    
    # Spawn main.exe with standard subprocess popen
    proc = subprocess.Popen(
        [exe_path],
        stdin=subprocess.PIPE,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        text=True,
        bufsize=0
    )

    out_queue = Queue()

    # Dedicated thread to continuously read output from main.exe
    def enqueue_output(out, queue):
        try:
            while True:
                char = out.read(1)
                if not char:
                    break
                queue.put(char)
        except Exception:
            pass

    t = threading.Thread(target=enqueue_output, args=(proc.stdout, out_queue), daemon=True)
    t.start()

    # Send queued output characters down the WebSocket
    async def send_output():
        try:
            while proc.poll() is None or not out_queue.empty():
                try:
                    # Non-blocking pull from queue
                    char = out_queue.get_nowait()
                    await websocket.send_text(char)
                except Empty:
                    await asyncio.sleep(0.01)
        except Exception:
            pass

    sender_task = asyncio.create_task(send_output())

    try:
        while proc.poll() is None:
            user_input = await websocket.receive_text()
            if proc.stdin and proc.poll() is None:
                proc.stdin.write(f"{user_input}\n")
                proc.stdin.flush()
    except WebSocketDisconnect:
        pass
    finally:
        if proc.poll() is None:
            proc.terminate()
        await sender_task