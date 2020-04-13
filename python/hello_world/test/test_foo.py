import unittest

from hw import foo


class HwFooTests(unittest.TestCase):

    def test_hw_bar(self):
        self.assertEqual(foo.bar(4), 16 )
        self.assertEqual(foo.bar(3), 9 )


    def test_hw_cube(self):
        self.assertEqual(foo.cube(3), 27 )
        self.assertEqual(foo.cube(2), 8 )

    def test_hw_cube_anothertest(self):
        self.assertEqual(foo.cube(3), 27 )
        self.assertEqual(foo.cube(2), 8 )



    def test_hw_string_letter_count(self):
        str="hello world, its a crazy world right now"

        num = 0
        for char in str:
            num += 1

        self.assertEqual(num, len(str))
        self.assertEqual(num, 40)





if __name__ == '__main__':
    unittest.main()
