# Security Policy

## Supported Versions

We release patches for security vulnerabilities in the following versions:

| Version | Supported          |
| ------- | ------------------ |
| 0.2.x   | :white_check_mark: |
| 0.1.x   | :x:                |
| < 0.1   | :x:                |

## Reporting a Vulnerability

We take security seriously. If you discover a security vulnerability, please follow these steps:

### 🔒 Private Disclosure

**DO NOT** open a public issue for security vulnerabilities. Instead:

1. **Email** the security team at: security@freshvoxelengine.org (or create a private security advisory)
2. **Include** as much information as possible:
   - Type of vulnerability
   - Steps to reproduce
   - Potential impact
   - Suggested fix (if any)

### 📧 What to Include

Please provide:

- **Description**: Clear description of the vulnerability
- **Steps to Reproduce**: Detailed steps to reproduce the issue
- **Impact**: What can an attacker achieve?
- **Environment**: OS, compiler, GPU, etc.
- **Proof of Concept**: Code or screenshots (if applicable)
- **Suggested Fix**: If you have ideas

### ⏱️ Response Timeline

- **Initial Response**: Within 48 hours
- **Status Update**: Within 7 days
- **Fix Timeline**: Depends on severity
  - Critical: 1-7 days
  - High: 7-30 days
  - Medium: 30-90 days
  - Low: Best effort

### 🏆 Recognition

Security researchers who responsibly disclose vulnerabilities will be:

- Credited in the security advisory (unless anonymous preferred)
- Listed in our security hall of fame
- Mentioned in release notes

## Security Update Process

1. **Acknowledgment**: We confirm receipt of your report
2. **Investigation**: We investigate and reproduce the issue
3. **Fix Development**: We develop and test a fix
4. **Notification**: We notify you when fix is ready
5. **Release**: We release a patched version
6. **Disclosure**: We publish a security advisory

## Security Best Practices

### For Users

- ✅ Always use the latest stable version
- ✅ Keep Vulkan drivers updated
- ✅ Validate downloaded asset packs
- ✅ Be cautious with community scripts
- ✅ Run with minimal necessary privileges

### For Developers

- ✅ Follow secure coding guidelines
- ✅ Validate all external input
- ✅ Use safe string operations
- ✅ Avoid buffer overflows
- ✅ Sanitize file paths
- ✅ Use RAII for resource management

## Known Security Considerations

### Asset Loading

- **Risk**: Malicious asset packs could contain harmful data
- **Mitigation**: Validate asset pack manifests, sandbox asset loading
- **Status**: 🚧 In development

### Scripting System

- **Risk**: User scripts could access system resources
- **Mitigation**: Sandbox script execution, limit API access
- **Status**: 🚧 Planned for future release

### Network (Multiplayer)

- **Risk**: Network vulnerabilities in multiplayer
- **Mitigation**: Encrypt traffic, validate packets, rate limiting
- **Status**: 🚧 Not yet implemented

### Shader Compilation

- **Risk**: Malicious shaders could cause GPU crashes
- **Mitigation**: Validate shader code, use trusted compilers
- **Status**: ✅ Using Vulkan validation layers

## CVE Assignment

For significant vulnerabilities, we will:

- Request a CVE number
- Publish a CVE entry
- Update this security policy
- Notify downstream projects

## Security Contacts

- **Email**: security@freshvoxelengine.org
- **GitHub**: [@shifty81](https://github.com/shifty81)
- **PGP Key**: [Coming soon]

## Hall of Fame

Security researchers who have helped us:

<!-- Will be populated as reports come in -->

*No reports yet - be the first!*

## Legal

We follow the principle of **coordinated disclosure**. We will work with you to understand and fix the issue before public disclosure.

## Additional Resources

- [OWASP Secure Coding Practices](https://owasp.org/www-project-secure-coding-practices-quick-reference-guide/)
- [CWE Top 25](https://cwe.mitre.org/top25/)
- [Vulkan Security Best Practices](https://www.vulkan.org/)

---

**Last Updated**: 2025-11-03  
**Version**: 1.0
