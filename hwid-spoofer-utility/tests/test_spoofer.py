import sys
import os
import unittest
from unittest.mock import patch, MagicMock

# Add src to path
sys.path.insert(0, os.path.join(os.path.dirname(__file__), '..', 'src'))
from spoofer import HwidSpoofer
from backup import HwidBackup

class TestHwidSpoofer(unittest.TestCase):
    def setUp(self):
        self.spoofer = HwidSpoofer(verbose=False)

    def test_generate_spoof_length(self):
        result = self.spoofer._generate_spoof(32)
        self.assertEqual(len(result), 32)
        self.assertTrue(all(c in '0123456789ABCDEF' for c in result))

    def test_generate_spoof_default_length(self):
        result = self.spoofer._generate_spoof()
        self.assertEqual(len(result), 16)

    @patch('winreg.OpenKey')
    @patch('winreg.QueryValueEx')
    @patch('winreg.SetValueEx')
    def test_spoof_machine_guid_success(self, mock_set, mock_query, mock_open):
        mock_query.return_value = ("original-guid", 1)
        mock_open.return_value.__enter__.return_value = MagicMock()
        result = self.spoofer.spoof_machine_guid()
        self.assertIsNotNone(result)
        self.assertEqual(len(result), 32)
        self.assertIn("MachineGuid", self.spoofer._original_values)

    @patch('winreg.OpenKey', side_effect=PermissionError)
    def test_spoof_machine_guid_permission_error(self, mock_open):
        result = self.spoofer.spoof_machine_guid()
        self.assertIsNone(result)

class TestHwidBackup(unittest.TestCase):
    def setUp(self):
        self.backup = HwidBackup(backup_dir="test_backups")

    def tearDown(self):
        import shutil
        if os.path.exists("test_backups"):
            shutil.rmtree("test_backups")

    @patch('winreg.OpenKey')
    @patch('winreg.QueryValueEx')
    def test_backup_machine_guid(self, mock_query, mock_open):
        mock_query.return_value = ("test-guid-1234", 1)
        mock_open.return_value.__enter__.return_value = MagicMock()
        result = self.backup.backup_machine_guid()
        self.assertIsNotNone(result)
        self.assertTrue(result.endswith(".json"))
        self.assertTrue(os.path.exists(result))

    def test_list_backups_empty(self):
        self.assertEqual(self.backup.list_backups(), [])

    def test_restore_from_nonexistent_backup(self):
        self.assertFalse(self.backup.restore_from_backup("nonexistent.json"))

if __name__ == "__main__":
    unittest.main()