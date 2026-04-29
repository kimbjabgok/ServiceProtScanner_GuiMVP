#include "details_panel.hpp"

namespace sps::gui {

DetailsPanel::DetailsPanel(QWidget* parent)
    : QTextEdit(parent)
{
    setReadOnly(true);
    setMinimumWidth(350);
    setPlaceholderText(QString::fromUtf8("\xe2\x86\x90 행을 클릭하면 상세 정보가 표시됩니다"));
}

void DetailsPanel::showResult(const core::ScanResult& r) {
    QString html;
    html += QString("<h3>%1:%2/%3 &mdash; %4</h3>")
        .arg(QString::fromStdString(r.target_host))
        .arg(r.port)
        .arg(QString::fromStdString(r.protocol))
        .arg(QString::fromStdString(r.service.name));

    html += QString("<b>Product:</b> %1 %2<br>")
        .arg(QString::fromStdString(r.service.product))
        .arg(QString::fromStdString(r.service.version));

    if (!r.service.extra_info.empty())
        html += QString("<b>Info:</b> %1<br>")
            .arg(QString::fromStdString(r.service.extra_info));

    html += QString("<b>RTT:</b> %1 ms<br><br>").arg(r.rtt.count());

    // Banner (HTML escape for XSS prevention, 512 char truncate)
    if (!r.service.banner_raw.empty()) {
        html += "<b>Banner:</b><pre style='background:#1a1a1a; padding:8px;'>";
        html += QString::fromStdString(r.service.banner_raw)
                    .toHtmlEscaped()
                    .left(512);
        html += "</pre><br>";
    }

    // Fingerprint section
    const auto& fp = r.fingerprint;
    if (!fp.ja4s.empty() || !fp.ja4x.empty() || !fp.cdn.empty() || !fp.os_guess.empty()) {
        html += "<h4>Fingerprint</h4>";
        html += "<div style='padding:6px; background:#252525; border-radius:4px; margin-bottom:8px;'>";
        if (!fp.ja4s.empty())
            html += QString("<b>JA4S:</b> <code>%1</code><br>")
                .arg(QString::fromStdString(fp.ja4s).toHtmlEscaped());
        if (!fp.ja4x.empty())
            html += QString("<b>JA4X:</b> <code>%1</code><br>")
                .arg(QString::fromStdString(fp.ja4x).toHtmlEscaped());
        if (!fp.cdn.empty())
            html += QString("<b>CDN:</b> %1<br>")
                .arg(QString::fromStdString(fp.cdn).toHtmlEscaped());
        if (!fp.os_guess.empty())
            html += QString("<b>OS:</b> %1<br>")
                .arg(QString::fromStdString(fp.os_guess).toHtmlEscaped());
        html += "</div>";
    }

    // CVE list
    if (!r.cves.empty()) {
        html += "<h4>Vulnerabilities</h4>";
        for (const auto& cve : r.cves) {
            QString color = cvss_html_color(cve.cvss_score);
            html += QString(
                "<div style='margin-bottom:8px; padding:6px; "
                "background:%1; border-radius:4px;'>"
                "<b>%2</b> &mdash; CVSS: %3 | EPSS: %4%5<br>"
                "<small>%6</small></div>")
                .arg(color)
                .arg(QString::fromStdString(cve.cve_id))
                .arg(cve.cvss_score, 0, 'f', 1)
                .arg(cve.epss_score, 0, 'f', 2)
                .arg(cve.nuclei_verified
                         ? QString::fromUtf8(" | \xe2\x9c\x93 Verified")
                         : QString())
                .arg(QString::fromStdString(cve.description));
        }
    } else {
        html += "<i>No known CVEs</i>";
    }

    setHtml(html);
}

QString DetailsPanel::cvss_html_color(float score) {
    if (score >= 9.0f) return "#4a1010";
    if (score >= 7.0f) return "#4a2a08";
    if (score >= 4.0f) return "#3a3508";
    return "#2a2a2a";
}

} // namespace sps::gui
