pipeline {
    agent any

    environment {
        PACKAGE_DIR = 'node_package'  // Directory to store the package
        BLN_PACKAGE_NAME = 'bln_node_package.zip'  // Name of the BLN node package (adjust as needed)
        GIT_BASH_PATH = 'C:\\Program Files\\Git\\bin\\bash.exe'  // Path to Git Bash
    }

    stages {
        stage('Checkout SCM') {
            steps {
                checkout scm
            }
        }

        stage('Build Node Package') {
            steps {
                script {
                    // List contents of the current directory for debugging
                    echo "Listing current directory contents..."
                    bat 'dir'

                    // Check if the label file exists
                    echo "Checking if label file exists in the current directory..."
                    if (fileExists("label")) {
                        echo "Label file found, copying to ${PACKAGE_DIR}"
                        // Ensure target directory exists before copying
                        bat "if not exist ${PACKAGE_DIR} mkdir ${PACKAGE_DIR}"
                        bat "copy label ${PACKAGE_DIR}\\"
                    } else {
                        error "Label file not found in the workspace!"
                    }
                }
            }
        }

        stage('Run build.sh using Git Bash') {
            steps {
                script {
                    echo "Running build.sh script using Git Bash..."

                    // Run the shell script using Git Bash
                    bat """
                        \"${GIT_BASH_PATH}\" build.sh
                    """
                }
            }
        }

        stage('Build BLN Node Package') {
            steps {
                script {
                    echo "Building BLN Node Package..."

                    // Assuming you have some steps to generate the BLN node package here
                    bat """
                        echo Compiling BLN node package...
                        # Replace this with the actual command to create the BLN node package
                        # Example command if it's a script:
                        build_bln_node_package.bat
                    """

                    // Verify if the BLN package is created
                    if (!fileExists("${PACKAGE_DIR}\\${BLN_PACKAGE_NAME}")) {
                        error "BLN Node Package not found!"
                    }
                }
            }
        }

        stage('Archive Package') {
            steps {
                archiveArtifacts artifacts: "${PACKAGE_DIR}/**/*", allowEmptyArchive: true
            }
        }

        stage('Clean Workspace') {
            steps {
                cleanWs()
            }
        }
    }

    post {
        always {
            echo "Cleaning up after pipeline execution"
        }

        success {
            echo "Pipeline executed successfully"
        }

        failure {
            echo "Pipeline failed"
        }
    }
}
