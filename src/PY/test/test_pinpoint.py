from unittest import TestCase
import unittest
import pinpoint
import sys


def output(msg):
    print(msg)

class TestAgent(TestCase):
     
    def setUp(self) -> None:
        self.assertTrue(pinpoint.set_collector(collector_host='unix:/tmp/collector-agent.sock'))
        self.assertTrue(pinpoint.enable_debug(output))

    def test_trace_life(self):
        self.assertEqual(pinpoint.start_trace(),1)
        self.assertEqual(pinpoint.start_trace(),2)
        self.assertEqual(pinpoint.start_trace(),3)
        self.assertEqual(pinpoint.start_trace(),4)
        pinpoint.add_clue("key","value")
        pinpoint.add_clue("key","value3")

        pinpoint.add_clues("key","values")
        pinpoint.add_clues("key","values")
        pinpoint.add_clues("key","values")

        self.assertEqual(pinpoint.end_trace(),3)
        self.assertEqual(pinpoint.end_trace(),2)
        self.assertEqual(pinpoint.end_trace(),1)
        self.assertEqual(pinpoint.end_trace(),0)

    def test_set_collector_host(self):
        self.assertTrue(pinpoint.set_collector(collector_host='unix:/tmp/collector1.sock'))
        self.assertTrue(pinpoint.set_collector(collector_host='Unix:/tmp/collector1.sock'))
        self.assertTrue(pinpoint.set_collector(collector_host='TCP:dev-collector:11331'))
        self.assertTrue(pinpoint.set_collector(collector_host='Tcp:dev-collector:11331'))

        try:
            pinpoint.set_collector(collector_host='dev-collector:11331')
            self.assertFalse(1)
        except:
            pass
        try:
            self.assertFalse(pinpoint.set_collector(collector_host='/tmp/collector1.sock'))
            self.assertFalse(1)
        except:
            pass
        
        self.assertTrue(pinpoint.set_collector(collector_host='Tcp:dev-collector:11331',trace_limit=1000))


# pinpoint.start_trace()
# pinpoint.end_trace()

if __name__ == '__main__':
    unittest.main()