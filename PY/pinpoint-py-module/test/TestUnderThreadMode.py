from unittest import TestCase
import unittest
from threading import Thread
import time
import pinpoint


class TestUnderThreadMode(TestCase):

    def setUp(self) -> None:
        self.thread_running = True

    def _test_api_flow(self):
        self.assertTrue(pinpoint.set_collector_host('unix:/tmp/unexist.sock'))
        self.assertTrue(pinpoint.enable_debug(None))

        while self.thread_running:
            self.assertEqual(pinpoint.start_trace(),1)
            pinpoint.add_clue("key","value3")
            pinpoint.add_clues("key","value3")
            self.assertEqual(pinpoint.end_trace(),0)


    def test_thead_safe(self):
        thread1 = Thread(target=self._test_api_flow)
        thread2 = Thread(target=self._test_api_flow)
        thread3 = Thread(target=self._test_api_flow)
        thread1.start()
        thread2.start()
        thread3.start()
        time.sleep(3)
        self.thread_running = False
        thread1.join()
        thread2.join()
        thread3.join()

if __name__ == '__main__':
    unittest.main()