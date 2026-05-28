import os
import json
import winreg
from datetime import datetime
from typing import Dict, Any, Optional

class HwidBackup:
    """Backup and restore HWID registry values."""

    def __init__(self, backup_dir: str = "backups"):
        self.backup_dir = backup_dir
        os.makedirs(backup_dir, exist_ok=True)

    def backup_machine_guid(self) -> Optional[str]:
        """Backup current MachineGuid to a JSON file."""
        try:
            with winreg.OpenKey(winreg.HKEY_LOCAL_MACHINE, r"SOFTWARE\Microsoft\Cryptography", 0, winreg.KEY_READ) as key:
                guid, _ = winreg.QueryValueEx(key, "MachineGuid")
        except FileNotFoundError:
            return None

        timestamp = datetime.now().strftime("%Y%m%d_%H%M%S")
        filename = f"hwid_backup_{timestamp}.json"
        filepath = os.path.join(self.backup_dir, filename)

        data = {
            "MachineGuid": guid,
            "timestamp": timestamp
        }
        with open(filepath, "w") as f:
            json.dump(data, f, indent=2)
        return filepath

    def restore_from_backup(self, backup_file: str) -> bool:
        """Restore MachineGuid from a backup file."""
        if not os.path.exists(backup_file):
            return False
        with open(backup_file, "r") as f:
            data = json.load(f)
        guid = data.get("MachineGuid")
        if not guid:
            return False
        try:
            with winreg.OpenKey(winreg.HKEY_LOCAL_MACHINE, r"SOFTWARE\Microsoft\Cryptography", 0, winreg.KEY_WRITE) as key:
                winreg.SetValueEx(key, "MachineGuid", 0, winreg.REG_SZ, guid)
            return True
        except PermissionError:
            return False

    def list_backups(self) -> list:
        """List all backup files."""
        if not os.path.exists(self.backup_dir):
            return []
        files = [f for f in os.listdir(self.backup_dir) if f.startswith("hwid_backup_") and f.endswith(".json")]
        return sorted(files, reverse=True)