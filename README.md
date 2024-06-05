# SE4HPCproject

## Step 2 -- From build to release and manual job execution 

Focus now on the correct implementation of the matrix multiplication you
find in <https://github.com/SimoneReale/SE4HPC_project_part2>. This is a
parallel implementation that uses MPI and reads the matrices to be
multiplied from two files, matrixA.txt and matrixB.txt. In these files
the first row contains the matrix dimensions (number of rows and
columns), while the other rows contain the matrix itself.

Your task is to perform the following steps:

**Preparation**: Use the template available here
<https://github.com/SimoneReale/SE4HPC_project_part2> to create your own
github repository. Add to this repository the tests you have created in
Step1.

**Automating the build, test and release processes**: Create a CI/CD
pipeline that, when someone pushes files in the repo, executes the
building and testing process.

**Containerizing the application**: Go through the following steps:

-   Define a Singularity container descriptor for the matrix
    multiplication program and push it in your repo.

-   Extend the created action to create a container image from your
    description.

**Executing on the cluster**: Go through the following steps:

-   Create a job.sh file to run your containerized application. Make
    sure that the standard output and error are mapped to txt files.

-   Transfer on Galileo100 your job script and the container.

-   Submit your job to the cluster and check whether it works correctly.

-   Push on your github repository your job.sh file and the files
    obtained from the execution of the matrix multiplication.

## Step 3 -- Automating a job submission with containerization 

Extend the action you have created at step 3 to automate completely the
process from a push on the repository to the execution of the
containerized software on SLURM. To do so, you will have to move your
container from the runner to the cluster. You can either use the scp
command or you can publish your image on the Singularity registry and
then pull it from the cluster. Don't forget to handle your secrets
properly! You do not want to leave passwords and authentication tokens
visible to everybody, so you will use the [secrets
mechanism](https://docs.github.com/en/actions/security-guides/using-secrets-in-github-actions?tool=cli).


# Short pipeline explanation and design decisions
The github pipeline builds the matrix multiplication application in a singularity container.

## Singularity
The `singularity` program can be compiled from source or installed via a .deb package. Whe chose the package solution to improve pipeline latency, but this solution could have maintenance problems in the future because we hardcoded the package version to the particular Ubuntu version of the runner. This solution could work if the singularity package were availabe in the apt repositories (and working).

## Upload files to the cluster
We used `sshpass` program to access ssh services without user interaction and provided the password with github secrets.
We decided to add also the username as a secret (but the username could be visible in previous commits). Some attention was necessary to pass the cluster controls in particular the options:
 * StrictHostKeyChecking=no
 * UserKnownHostsFile=/dev/null

## Launching the job on the cluster
With `ssh` the runner launches the compiled program on the cluster. We had trouble in making MPI working correctly, here is our trials:
 * (first implementation, wrong): `srun singularity run` + `mpirun -n 2 ./main` in the container
 * (more correct implementation): `mpirun -n 2 singularity exec ..`

The key point here is give to slurm the information of how many process our application needs and make the two MPI instances (for host and container) work together. In fact we had to bind temporary directories.

Another important point is that in this way the program reads the host filesystem, and we had trouble finding the matrix files which were present in the container image but not in the root folder from which the program was called.

A discussion with the tutor pointed that containers are used for managing the program build, but not always for storing the working files and it's a common practice to leave the input files outside the image.

We introduced a sleep in the runner script to wait for the job to complete before copying back the results. It's a duct tape solution, I'm sure there exists better ones.


## Dealing with the results of the computation
In previous versions of the repository (65aa48b) we made the action commit the resulting files into the repository. It was a nice solution but suffered from this simple problem: if the resulting files were already equal to the files in the worktree we couldn't commit no changes. In the end we chose the upload artifact action.
