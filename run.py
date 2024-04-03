#! python

import pathlib2
import yaml
import typing as t
from mip import Model, CBC
import os
import pandas as pd


class FeatureMatrixBuilder:
    """
    Строит признаковое пространство объекта
    """

    def __init__(self, path_to_config_file: pathlib2.Path):
        self.config_dict: dict = self.read_config_yaml_file(path_to_config_file)

        if (
            "path_to_sol_files" not in self.config_dict["paths"]
            or not self.config_dict["paths"]["path_to_sol_files"]
        ):
            self._write_target = False
        else:
            self._write_target: bool = True
            self._path_to_sol_files_dir = pathlib2.Path(
                self.config_dict["paths"]["path_to_sol_files"]
            )

        self._path_to_lp_files_dir = pathlib2.Path(
            self.config_dict["paths"]["path_to_milp_files"]
        )
        self._path_to_lp_relaxed_files_dir = pathlib2.Path(
            self.config_dict["paths"]["path_to_lp_files"]
        )
        self._path_to_output_dir = pathlib2.Path(
            self.config_dict["paths"]["path_to_output_dir"]
        )
        self._path_to_exe = pathlib2.Path(self.config_dict["paths"]["path_to_exe"])

        if self._write_target:
            self._args_for_c: t.List[
                t.Tuple[pathlib2.Path, pathlib2.Path, pathlib2.Path, str]
            ] = []

            for lp_file in self._path_to_lp_files_dir.iterdir():
                for sol_file in self._path_to_sol_files_dir.iterdir():
                    lp_file_name = lp_file.name.split(".")[0]
                    if lp_file_name in sol_file.name:
                        rel_lp_file = self.write_relaxed_problem(
                            self._path_to_lp_relaxed_files_dir, lp_file
                        )
                        self._args_for_c.append(
                            (lp_file, rel_lp_file, sol_file, lp_file_name)
                        )
        else:
            self._args_for_c: t.List[t.Tuple[pathlib2.Path, pathlib2.Path, str]] = []
            for lp_file in self._path_to_lp_files_dir.iterdir():
                lp_file_name = lp_file.name.split(".")[0]
                rel_lp_file = self.write_relaxed_problem(
                    self._path_to_lp_relaxed_files_dir, lp_file
                )
                self._args_for_c.append((lp_file, rel_lp_file, lp_file_name))

    @staticmethod
    def read_config_yaml_file(path_to_config_file: pathlib2.Path) -> dict:
        """
        Читает конфигурационный yaml-файл
        """
        try:
            with open(path_to_config_file, encoding="utf-8") as file:
                config_yaml = yaml.safe_load(file)
        except FileNotFoundError as err:
            print(f"{err}")
        else:
            print(f"File {path_to_config_file} has been read successfully.")

        return config_yaml

    @staticmethod
    def write_relaxed_problem(
        relaxed_lp_file_dir: pathlib2.Path, path_to_lp_file: pathlib2.Path
    ) -> str:
        """
        записывает релаксированную задачу в lp-файл
        """
        lp_name = path_to_lp_file.name.split(".")[0]
        rel_file_name = f"{relaxed_lp_file_dir}\{lp_name}_relaxed.lp"
        model = Model(solver_name=CBC)
        model.read(str(path_to_lp_file))
        model.relax()
        model.write(rel_file_name)
        print(f"Relaxed problem {path_to_lp_file} has been written successfully.")
        return rel_file_name

    def write_features(self) -> None:
        features_config = " ".join(
            [str(int(dct["use"])) for dct in self.config_dict["features"].values()]
        )
        if self._write_target:
            for lp_file, rel_lp_file, sol_file, scenario_name in self._args_for_c:
                returncode = os.system(
                    f"{self._path_to_exe} 1 {sol_file} {lp_file} {rel_lp_file} {scenario_name} "
                    f"{self._path_to_output_dir} {features_config}"
                )

                features_file_name = (
                    f"{self._path_to_output_dir}\\features_{scenario_name}.csv"
                )
                rel_features_file_name = (
                    f"{self._path_to_output_dir}\\rel_features_{scenario_name}.csv"
                )
                taget_file_name = (
                    f"{self._path_to_output_dir}\\target_{scenario_name}.csv"
                )

                features_df = pd.read_csv(features_file_name, index_col="var")
                rel_features_df = pd.read_csv(
                    rel_features_file_name, index_col="var_rel"
                )
                target_df = pd.read_csv(
                    taget_file_name, index_col="var_target"
                )

                main_df = (
                    pd.concat([features_df, rel_features_df, target_df], axis=1)
                    .reset_index()
                    .rename(columns={"index": "var"})
                )
                # main_df = main_df.drop(["var_rel", "var_target"], axis=0)

                pathlib2.Path(features_file_name).unlink()
                pathlib2.Path(rel_features_file_name).unlink()
                pathlib2.Path(taget_file_name).unlink()

                main_df.to_csv(features_file_name, index=False)

                print(f"File {features_file_name} has been written successfully.")
        else:
            for lp_file, rel_lp_file, scenario_name in self._args_for_c:
                os.system(
                    f"{self._path_to_exe} 0 {lp_file} {rel_lp_file} {scenario_name} "
                    f"{self._path_to_output_dir} {features_config}"
                )

                features_file_name = (
                    f"{self._path_to_output_dir}/features_{scenario_name}.csv"
                )
                rel_features_file_name = (
                    f"{self._path_to_output_dir}/rel_features_{scenario_name}.csv"
                )
                features_df = pd.read_csv(features_file_name, index_col="var")
                rel_features_df = pd.read_csv(
                    rel_features_file_name, index_col="var_rel"
                )
                main_df = (
                    pd.concat([features_df, rel_features_df], axis=1)
                    .reset_index()
                    .rename(columns={"index": "var"})
                )

                pathlib2.Path(features_file_name).unlink()
                pathlib2.Path(rel_features_file_name).unlink()

                main_df.to_csv(features_file_name, index=False)

                print(f"File {features_file_name} has been written successfully.")


if __name__ == "__main__":
    fmb = FeatureMatrixBuilder("./config.yaml")
    fmb.write_features()
