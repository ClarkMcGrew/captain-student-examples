#include <memory>
#include <iostream>
#include <fstream>

#include <eventLoop.hxx>

#include <THandle.hxx>
#include <TG4Trajectory.hxx>

/// User code for the event loop.  This can be copied and used as a skeleton
/// for generic user loop programs.
class TUserLoop: public CP::TEventLoopFunction {
public:
    /// Initialize any class specific variables, but most of the work can be
    /// done in Initialize.  Don't create histograms here!
    TUserLoop() {}
    virtual ~TUserLoop() {};

    /// Print a usage message.  This is generally called when there is a
    /// command line input error.
    void Usage(void) {
    }

    /// Set an option and return true if it is valid.  This is called by the
    /// event loop command line argument processing code for each "-O
    /// [name]=[value]" option found on the command line.  If the command line
    /// has "-O [name]" without a value, then the value string will be equal
    /// to "".  This must return false if the option was not correctly
    /// processed.
    virtual bool SetOption(std::string option,std::string value="") {
        return false;
    }

    /// Called for each event inside the event loop, and returns true if the
    /// event should be saved to the output file.  If the remainder of the
    /// current file should be skipped, this should through the
    /// ENextEventLoopFile exception.  If the daughter class defines this
    /// operator, it should not define the Process() method.
    bool operator () (CP::TEvent& event) {
        CP::THandle<CP::TG4TrajectoryContainer> trajectories
            = event.Get<CP::TG4TrajectoryContainer>("truth/G4Trajectories");
        if (!trajectories) {
            std::cout << "No Trajectory container" << std::endl;
            return false;
        }
        trajectories->ls();
        CP::THandle<CP::TG4Trajectory> traj = trajectories->GetTrajectory(1);
        if (!traj) {
            std::cout << "No Trajectories" << std::endl;
        }
        std::cout << "Print the trajectory" << std::endl;
        traj->ls();
        std::cout << "Get the initial position" << std::endl;
        std::cout << " x " << traj->GetInitialPosition().X() << std::endl;
        std::cout << " y " << traj->GetInitialPosition().Y() << std::endl;
        std::cout << " z " << traj->GetInitialPosition().Z() << std::endl;
        std::cout << "Get the final position" << std::endl;
        std::cout << " x " << traj->GetFinalPosition().X() << std::endl;
        std::cout << " y " << traj->GetFinalPosition().Y() << std::endl;
        std::cout << " z " << traj->GetFinalPosition().Z() << std::endl;

        double dx = traj->GetFinalPosition().X()-traj->GetInitialPosition().X();
        double dy = traj->GetFinalPosition().Y()-traj->GetInitialPosition().Y();
        double dz = traj->GetFinalPosition().Z()-traj->GetInitialPosition().Z();

        double length = std::sqrt(dx*dx + dy*dy + dz*dz);

        std::cout << length << std::endl;

        fOutput << length << std::endl;
        
        return true;
    }

    /// Called after the arguments are processed but before reading the first
    /// event.  The output file is open so any histograms will be added to the
    /// output file.
    virtual void Initialize(void) {
        fOutput.open("proton100.data");
    }

    /// Called after reading the last event.  The output file is still open,
    /// so you can add extra information.  Because of an ideosyncracy in the
    /// way root handles histograms, objects created in Initialize() will
    /// already be stored in the output file.
    virtual void Finalize(CP::TRootOutput* output) {
        fOutput.close();
    }

private:

    std::ofstream fOutput;
    
};

int main(int argc, char **argv) {
    TUserLoop userLoop;
    CP::eventLoop(argc,argv,userLoop);
}
