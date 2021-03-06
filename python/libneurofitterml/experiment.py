from lxml.builder import E
from . import white_space_delim


class Experiment(object):

    element_name = 'Experiment'

    def to_xml(self):
        if isinstance(self, FileListExperiment):
            type_xml = E(self.element_name + 'Type', 'FileList')
        elif isinstance(self, FakeExperiment):
            type_xml = E(self.element_name + 'Type', 'Fake')
        else:
            assert(False), "The class '{}' is not listed in Experiment.to_xml()".format(type(self))
        return (type_xml, self._to_xml())

    @classmethod
    def from_xml(cls, type_element, parameters_element):
        experiment_type = type_element.text.strip()
        if experiment_type == 'FileList':
            experiment = FileListExperiment.from_xml(parameters_element)
        elif experiment_type == 'Fake':
            experiment = FakeExperiment.from_xml(parameters_element)
        else:
            raise Exception("Unrecognised type '{}' of traces reader".format(experiment_type))
        return experiment


class FileExperiment(Experiment):

    def __init__(self, files_dir):
        """
        This reads the experimental traces from files with fixed filenames. The filename should be 
        of the form output_Run0.dat, output_Run1.dat, output_Run2.dat, ...  where "output" is the 
        prefix as specified in the NormalTracesReader. There should be 1 file for every run (1 run 
        is e.g. 1 current injection strength).
        
        `files_dir` -- Specifies where the directory with the experimental data is 
        """
        self.files_dir = files_dir

    def _to_xml(self):
        return E(self.element_name + 'Parameters',
                 E('FilesDirectory', self.files_dir))

    @classmethod
    def from_xml(cls, element):
        files_dir = element.find('FilesDirectory').text.strip()
        return cls(files_dir)

    @property
    def num_runs(self):
        return len(self.files_list)


class FileListExperiment(Experiment):

    def __init__(self, files_list):
        """
        This reads the experimental traces from files with filenames specified by the user.
        
        `files_list` -- The number of files should be the same as the "num_runs" of the TracesReader 
        """
        self.files_list = files_list

    def _to_xml(self):
        return E(self.element_name + 'Parameters',
                 E('FilesList', '\n'.join(self.files_list)))

    @classmethod
    def from_xml(cls, element):
        parameters = [p.strip() for p in element.find('FilesList').text.strip().split('\n')]
        return cls(parameters)

    @property
    def num_runs(self):
        return len(self.files_list)


class FakeExperiment(Experiment):

    def __init__(self, parameters):
        """
        This type will not read the experimental data from files but will generate "fake" experimental 
        data based on running the model with a set of parameters you provide
        
        `parameters` -- A list containing the parameter set you want to generate the data with
        """
        self.parameters = parameters

    def _to_xml(self):
        return E(self.element_name + 'Parameters',
                 E('Parameters', ' '.join(self.parameters)))

    @classmethod
    def from_xml(cls, element):
        parameters = white_space_delim.split(element.find('Parameters').text.strip())
        return cls(parameters)
