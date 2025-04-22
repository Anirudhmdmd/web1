pipeline {
    agent any

    environment {
        PACKAGE_DIR = 'node_package'  // Directory to store the package
        BLN_PACKAGE_NAME = 'bln_node_package.el'  // Name of the BLN node package (adjust as needed)
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
                    // Check if the label file exists
                    echo "Checking if label file exists in the current directory..."
                    if (fileExists("label")) {
                        echo "Label file found, copying to ${PACKAGE_DIR}"
                        bat "copy label ${PACKAGE_DIR}\\"
                    } else {
                        error "Label file not found in the workspace!"
                    }
                }
            }
        }

        stage('Build BLN Node Package') {
            steps {
                script {
                    echo "Building BLN Node Package..."

                    // Assuming you have some steps to generate the BLN node package here
                    // Replace the following example with the actual commands to create the package
                    
                    // Example: Assuming the BLN package is created using a script or tool
                    bat """
                        echo Compiling BLN node package...
                        # Replace this with the actual command to create the BLN node package
                        # For example, if it's a custom script:
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
