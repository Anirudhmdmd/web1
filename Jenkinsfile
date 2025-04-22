pipeline {
    agent any

    environment {
        // Define the workspace where the files will be stored
        EL_HOME = 'C:\\Nokia\\Mediation\\elcompiler'
        PATH = "${EL_HOME}\\bin;${env.PATH}"
    }

    stages {
        stage('Checkout SCM') {
            steps {
                // Checkout the code from Git repository
                checkout scm
            }
        }

        stage('Build BLN Node Package') {
            steps {
                script {
                    echo 'Building BLN Node Package...'
                    // Run the batch script to compile the BLN EL Node
                    bat 'build_el_package.bat'
                }
            }
        }

        stage('Archive Package') {
            steps {
                // Archive the .el file as an artifact
                archiveArtifacts artifacts: '**/*.el', fingerprint: true
            }
        }

        stage('Clean Workspace') {
            steps {
                cleanWs() // Clean up the workspace after the build
            }
        }
    }

    post {
        always {
            echo 'Cleaning up after pipeline execution...'
        }
    }
}
