pipeline {
    agent any

    environment {
        PACKAGE_DIR = 'node_package'  // Directory to store the package
        BLN_PACKAGE_NAME = 'bln_node_package.zip'  // Name of the BLN node package (adjust as needed)
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

        stage('Run Build Script with WSL Bash') {
            steps {
                script {
                    echo "Running build.sh script using WSL Bash..."
                    // Run the build.sh script using WSL bash.exe (WSL should be installed)
                    bat "\"C:\\Windows\\System32\\bash.exe\" -c \"./build.sh\""
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
