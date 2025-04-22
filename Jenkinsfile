pipeline {
    agent any
    stages {
        stage('Checkout') {
            steps {
                git url: 'https://github.com/Anirudhmdmd/web1.git', branch: 'main'
            }
        }

        stage('Build Node Package') {
            steps {
                script {
                    // Create node_package directory if it doesn't exist
                    bat 'if not exist node_package mkdir node_package'

                    // List directory contents to dir_contents.txt
                    bat 'dir > dir_contents.txt'

                    // Print current directory path to current_dir.txt
                    bat 'echo Current directory: %cd% > current_dir.txt'

                    // Copy the label file if it exists
                    bat '''
                    if exist label (
                        copy label node_package\\
                    ) else (
                        echo ERROR: label file not found!
                        exit /b 1
                    )
                    '''
                }
            }
        }

        stage('Archive Package') {
            when {
                expression {
                    return fileExists('node_package')
                }
            }
            steps {
                // Archive the contents of node_package if it exists
                archiveArtifacts artifacts: 'node_package/**', fingerprint: true
            }
        }
    }

    post {
        always {
            // Clean workspace after build
            cleanWs()
        }
    }
}
