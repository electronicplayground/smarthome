import os

from configuration.config_keys import ConfigKeys

class EnvironmentVariablesMixin(object):
    _env_loaded = False

    def load_env(self):
        if self._env_loaded:
            return
        print("Loading Environment Configurations ...")
        for k in ConfigKeys.CONFIG_KEYS:
            print("Getting {} ...".format(k))
            v = os.environ.get(k)
            if v:
                self.items[k] = v
            else:
                default = ConfigKeys.DEFAULTS.get(k)
                print(f"Environment config not found for {k}. Default {default}")
                self.items[k] = default
        
        self._env_loaded = True


class Configuration(EnvironmentVariablesMixin):
    __instance__ = None

    PRINT_VALUES = (
    )

    def __init__(self):
        self.items = dict()
        self._local_tenants_config_loaded = False

        if not Configuration.__instance__:
            Configuration.__instance__ = self
        else:
            raise Exception("This class is a singleton.")

    def __getitem__(self, key):
        item = self.items.get(key, None)
        if not item:
            print(f"Could not find {key} in Configuration.")
        return item

    @staticmethod
    def Instance():
        instance = Configuration.__instance__
        if not instance:
            print("Creating configuration instance")
            Configuration()
            instance = Configuration.__instance__
        
        instance.load_env()
        
        return instance
