from unittest import TestCase
import unittest
from multiprocessing import Process
import time
import pinpoint


class TestUnderProcessMode(TestCase):
    def _test_api_flow(self):
        self.assertTrue(pinpoint.set_collector_host('unix:/tmp/unexist.sock'))
        self.assertTrue(pinpoint.enable_debug(None))

        while True:
            self.assertEqual(pinpoint.start_trace(),1)
            pinpoint.add_clue("key","value3")
            pinpoint.add_clues("key","value3")
            self.assertEqual(pinpoint.end_trace(),0)

    def test_process(self):
        p1 = Process(target=self._test_api_flow)
        p1.start()
        p2 = Process(target=self._test_api_flow)
        p2.start()
        p3 = Process(target=self._test_api_flow)
        p3.start()
        time.sleep(3)
        p1.terminate()
        p2.terminate()
        p3.terminate()
        p1.join()
        p2.join()
        p3.join()

if __name__ == '__main__':
    unittest.main()