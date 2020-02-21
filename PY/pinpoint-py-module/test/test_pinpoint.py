from unittest import TestCase
import unittest
import pinpoint
from io import StringIO
import sys



class TestImplement(TestCase):
    def setUp(self) -> None:
        print(pinpoint.enable_debug())

    def test_trace_life(self):
        self.assertEqual(pinpoint.start_trace(),1)
        self.assertEqual(pinpoint.start_trace(),2)
        self.assertEqual(pinpoint.start_trace(),3)
        self.assertEqual(pinpoint.start_trace(),4)

        pinpoint.add_clue("key","value");
        pinpoint.add_clue("key","value3");

        pinpoint.add_clues("key","values");
        pinpoint.add_clues("key","values");
        pinpoint.add_clues("key","values");

        self.assertEqual(pinpoint.end_trace(),3)
        self.assertEqual(pinpoint.end_trace(),2)
        self.assertEqual(pinpoint.end_trace(),1)
        self.assertEqual(pinpoint.end_trace(),0)


# pinpoint.start_trace()
# pinpoint.end_trace()

if __name__ == '__main__':
    unittest.main()